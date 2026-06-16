#ifndef MY_PRJ_H_
#define MY_PRJ_H_

#include "BDT.h"
#include "parameters.h"
#include "hls_stream.h"
#include "ap_axi_sdata.h"

// define stream types
typedef hls::axis<accelerator_input_t, 0, 0, 0>  input_stream_t;
typedef hls::axis<accelerator_output_t, 0, 0, 0> output_stream_t;

// Prototype of top level function for C-synthesis
void my_prj(
	input_arr_t data,
	score_arr_t score,
	score_t tree_scores[BDT::fn_classes(n_classes) * n_trees]);

// void my_prj_accelerator(
//     int N, 
//     /* int& n_f, int& n_c, */ 
//     accelerator_input_t* x, 
//     accelerator_output_t* score);

void my_prj_accelerator(
    int N,
    hls::stream<input_stream_t>&  x,
    hls::stream<output_stream_t>& score
);
#endif


