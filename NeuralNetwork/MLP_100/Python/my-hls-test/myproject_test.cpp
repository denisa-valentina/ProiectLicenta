#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// #include "firmware/defines.h"
#include "firmware/myproject.h"
#include "firmware/nnet_utils/nnet_helpers.h"
// #include "firmware/parameters.h"

// hls-fpga-machine-learning insert bram

#define CHECKPOINT 5000

namespace nnet {
bool trace_enabled = true;
std::map<std::string, void *> *trace_outputs = NULL;
size_t trace_type_size = sizeof(double);
} // namespace nnet

int main(int argc, char **argv) {

    std::ifstream fin("tb_data/tb_input_features.dat"); // load input data from text file
    std::ifstream fpr("tb_data/result_data.dat"); // load predictions from text file

    
#ifdef RTL_SIM
    std::string RESULTS_LOG = "tb_data/rtl_cosim_results.log";
#else
    std::string RESULTS_LOG = "tb_data/csim_results.log";
#endif
    std::ofstream fout(RESULTS_LOG);

    std::string iline;
    std::string pline;
    int e = 0;

    // flat buffers — one per vector at a time
    accelerator_input_t  flat_input[n_features];
    accelerator_output_t flat_output[1];

    int n_out = 1;
    std::vector<int> TP(n_out, 0), FP(n_out, 0), TN(n_out, 0), FN(n_out, 0);

    bool has_labels = fpr.is_open();
    if (!has_labels)
        std::cout << "WARNING: Label file not found. Metrics will not be computed." << std::endl;
    
    if (fin.is_open()) {
        while (std::getline(fin, iline)) {
            if (e % CHECKPOINT == 0)
                std::cout << "Processing input " << e << std::endl;

            // --- parse one line into flat_input ---
            char* cstr = const_cast<char*>(iline.c_str());
            char* current = strtok(cstr, " ");
            std::vector<float> in;
            int i = 0;
            while (current != NULL && i < n_features) {
                in.push_back((accelerator_input_t)atof(current));
                current = strtok(NULL, " ");
                i++;
            }

            if (i != n_features) {
                std::cout << "WARNING: line " << e
                          << " has " << i << " features, expected "
                          << n_features << std::endl;
            }
            // --- fill HLS stream with this vector ---
            hls::stream<input_axi_t> input_stream("input_stream");
            hls::stream<result_axi_t> output_stream("output_stream");

            for (int j = 0; j < n_features; j++) {
                input_axi_t pkt;
                pkt.data = in[j];
                pkt.keep = -1;
                pkt.strb = -1;
                pkt.last = (j == n_features - 1);
                input_stream.write(pkt);
            }

            // --- call accelerator with N=1 ---
            my_prj_accelerator(1, input_stream, output_stream);

            // --- read output from stream ---
            for (int j = 0; j < n_out; j++) {
                result_axi_t pkt_out;
                pkt_out = output_stream.read();
                flat_output[j] = pkt_out.data;
            }

            // --- apply threshold: score > 0.5 → 1, altfel → 0 ---
            std::vector<int> pred(n_out);
            for (int j = 0; j < n_out; j++) {
                pred[j] = (flat_output[j] > 0.5f) ? 1 : 0;
            }

            // --- write result to file ---
            for (int j = 0; j < n_out; j++) {
                fout << pred[j] << std::endl;
            }

            // --- update confusion matrix vs. ground truth ---
            if (has_labels) {
                std::string lline;
                if (std::getline(fpr, lline)) {
                    // parse ground-truth labels (one per class, space-separated)
                    char* lcstr = const_cast<char*>(lline.c_str());
                    char* ltok  = strtok(lcstr, " ");
                    for (int j = 0; j < n_out && ltok != NULL; j++) {
                        int gt = atoi(ltok);
                        int pr = pred[j];

                        if      (pr == 1 && gt == 1) TP[j]++;
                        else if (pr == 1 && gt == 0) FP[j]++;
                        else if (pr == 0 && gt == 0) TN[j]++;
                        else if (pr == 0 && gt == 1) FN[j]++;

                        ltok = strtok(NULL, " ");
                    }
                }
            }

            // --- print checkpoint ---
            if (e % CHECKPOINT == 0) {
                std::cout << "Result[" << e << "]: ";
                for (int j = 0; j < n_out; j++)
                    std::cout << pred[j] << " ";
                std::cout << std::endl;
            }

            e++;
        }

        fin.close();
        std::cout << "Total vectors processed: " << e << std::endl;

    }

    // ---- Compute & display metrics ----
    if (has_labels && e > 0) {
        std::cout << "\n========== METRICS ==========\n";
        std::cout << std::fixed << std::setprecision(4);

        // accumulatori globali pentru macro-average
        double sum_acc = 0, sum_prec = 0, sum_rec = 0, sum_f1 = 0;

        for (int j = 0; j < n_out; j++) {
            int total = TP[j] + FP[j] + TN[j] + FN[j];

            double accuracy  = (total > 0)
                               ? (double)(TP[j] + TN[j]) / total
                               : 0.0;

            double precision = (TP[j] + FP[j] > 0)
                               ? (double)TP[j] / (TP[j] + FP[j])
                               : 0.0;

            double recall    = (TP[j] + FN[j] > 0)
                               ? (double)TP[j] / (TP[j] + FN[j])
                               : 0.0;

            double f1        = (precision + recall > 0)
                               ? 2.0 * precision * recall / (precision + recall)
                               : 0.0;

            std::cout << "--- Class " << j << " ---\n";
            std::cout << "  TP=" << TP[j] << "  FP=" << FP[j]
                      << "  TN=" << TN[j] << "  FN=" << FN[j] << "\n";
            std::cout << "  Accuracy  = (TP+TN)/(TP+FP+TN+FN) = "
                      << accuracy  << "\n";
            std::cout << "  Precision = TP/(TP+FP)             = "
                      << precision << "\n";
            std::cout << "  Recall    = TP/(TP+FN)             = "
                      << recall    << "\n";
            std::cout << "  F1 Score  = 2*P*R/(P+R)            = "
                      << f1        << "\n\n";

            sum_acc  += accuracy;
            sum_prec += precision;
            sum_rec  += recall;
            sum_f1   += f1;
        }

        // macro-average peste toate clasele
        std::cout << "--- Macro-Average (all " << n_out << " classes) ---\n";
        std::cout << "  Accuracy  = " << sum_acc  / n_out << "\n";
        std::cout << "  Precision = " << sum_prec / n_out << "\n";
        std::cout << "  Recall    = " << sum_rec  / n_out << "\n";
        std::cout << "  F1 Score  = " << sum_f1   / n_out << "\n";
        std::cout << "==============================\n";

        fpr.close();
    }

    fout.close();
    std::cout << "INFO: Saved results to: " << RESULTS_LOG << std::endl;

    return 0;
}
