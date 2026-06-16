//
//    rfnoc-hls-neuralnet: Vivado HLS code for neural-net building blocks
//
//    Copyright (C) 2017 EJ Kreinar
//
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "my_prj.h"
#include "parameters.h"
#include "BDT.h"

#define CHECKPOINT 5000

int main(int argc, char **argv)
{
    // load input data from text file
    std::ifstream fin("tb_data/tb_input_features.dat");
    std::ifstream flabels("tb_data/result_data.dat"); // ground truth labels

#ifdef RTL_SIM
    std::string RESULTS_LOG = "tb_data/cosim_results.log";
    std::string VERBOSE_LOG = "tb_data/cosim_tree_results.log";
#else
    std::string RESULTS_LOG = "tb_data/csim_results.log";
    std::string VERBOSE_LOG = "tb_data/csim_tree_results.log";
#endif

    std::ofstream fout(RESULTS_LOG);
    std::ofstream ftrees(VERBOSE_LOG);

    std::string iline;
    int e = 0;

    // flat buffers — one per vector at a time
    accelerator_input_t  flat_input[n_features];
    accelerator_output_t flat_output[BDT::fn_classes(n_classes)];

    // ---- Metrics accumulators (per class) ----
    int n_out = BDT::fn_classes(n_classes);
    std::vector<int> TP(n_out, 0), FP(n_out, 0), TN(n_out, 0), FN(n_out, 0);

    bool has_labels = flabels.is_open();
    if (!has_labels)
        std::cout << "WARNING: Label file not found. Metrics will not be computed." << std::endl;

    if (fin.is_open()) {
        while (std::getline(fin, iline)) {
            if (e % CHECKPOINT == 0)
                std::cout << "Processing input " << e << std::endl;

            // --- parse one line into flat_input ---
            char* cstr = const_cast<char*>(iline.c_str());
            char* current = strtok(cstr, " ");
            int i = 0;
            while (current != NULL && i < n_features) {
                flat_input[i] = (accelerator_input_t)atof(current);
                current = strtok(NULL, " ");
                i++;
            }

            if (i != n_features) {
                std::cout << "WARNING: line " << e
                          << " has " << i << " features, expected "
                          << n_features << std::endl;
            }

            // --- fill HLS stream with this vector ---
            hls::stream<input_stream_t> input_stream("input_stream");
            hls::stream<output_stream_t> output_stream("output_stream");

            for (int j = 0; j < n_features; j++) {
                input_stream_t pkt;
                pkt.data = flat_input[j];
                pkt.keep = -1;
                pkt.strb = -1;
                pkt.last = (j == n_features - 1);
                input_stream.write(pkt);
            }

            // --- call accelerator with N=1 ---
            my_prj_accelerator(1, input_stream, output_stream);

            // --- read output from stream ---
            for (int j = 0; j < n_out; j++) {
                output_stream_t pkt_out;
                pkt_out = output_stream.read();
                flat_output[j] = pkt_out.data;
            }

            // --- apply threshold: score < 0.5 → 1, altfel → 0 ---
            std::vector<int> pred(n_out);
            for (int j = 0; j < n_out; j++) {
                pred[j] = (flat_output[j] < 0.5f) ? 1 : 0;
            }

            // --- write result to file ---
            for (int j = 0; j < n_out; j++) {
                fout << pred[j] << std::endl;
            }

            // --- update confusion matrix vs. ground truth ---
            if (has_labels) {
                std::string lline;
                if (std::getline(flabels, lline)) {
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

        flabels.close();
    }

    fout.close();
    ftrees.close();
    std::cout << "INFO: Saved results to: " << RESULTS_LOG << std::endl;

    return 0;
}