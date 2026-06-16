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

#include "firmware/my_prj.h"
#include "firmware/parameters.h"
#include "firmware/BDT.h"

#define CHECKPOINT 5000

int main(int argc, char **argv)
{
    // load input data from text file
    std::ifstream fin("tb_data/tb_input_features.dat");

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
            for (int j = 0; j < BDT::fn_classes(n_classes); j++) {
                output_stream_t pkt_out;
                pkt_out = output_stream.read();
                flat_output[j] = pkt_out.data;
            }

            // --- write result to file ---
            for (int j = 0; j < BDT::fn_classes(n_classes); j++) {
                fout << std::fixed << std::setprecision(20)
                     << flat_output[j] << std::endl;
            }

            // --- print checkpoint ---
            if (e % CHECKPOINT == 0) {
                std::cout << "Result[" << e << "]: ";
                for (int j = 0; j < BDT::fn_classes(n_classes); j++) {
                    std::cout << flat_output[j] << " ";
                }
                std::cout << std::endl;
            }

            e++;
        }

        fin.close();
        std::cout << "Total vectors processed: " << e << std::endl;

    } 
    // else {
    //     // --- default zero input if file not found ---
    //     std::cout << "INFO: Unable to open input file, using default input." << std::endl;

    //     hls::stream<accelerator_input_t>  input_stream("input_stream");
    //     hls::stream<accelerator_output_t> output_stream("output_stream");

    //     // fill stream with zeros
    //     for (int j = 0; j < n_features; j++) {
    //         input_stream.write((accelerator_input_t)0.0f);
    //     }

    //     // call accelerator with N=1
    //     my_prj_accelerator(1, input_stream, output_stream);

    //     // read and print result
    //     for (int j = 0; j < BDT::fn_classes(n_classes); j++) {
    //         flat_output[j] = output_stream.read();
    //         std::cout << "score[" << j << "] = " << flat_output[j] << std::endl;
    //         fout << std::fixed << std::setprecision(20)
    //              << flat_output[j] << std::endl;
    //     }
    // }

    fout.close();
    ftrees.close();
    std::cout << "INFO: Saved results to: " << RESULTS_LOG << std::endl;

    return 0;
}