#include "BDT.h"
#include "parameters.h"
#include <iostream>
#include "my_prj.h"

void my_prj(input_arr_t x, score_arr_t score){
  #pragma HLS array_partition variable=x
  #pragma HLS array_partition variable=score
  #pragma HLS pipeline
  #pragma HLS unroll
  bdt.decision_function(x, score);
}

void copy_input(hls::stream<input_stream_t>& x_in, input_arr_t x_int) {
    #pragma HLS inline
    for (int i = 0; i < n_features; i++) {
        #pragma HLS pipeline
        input_stream_t val = x_in.read();
        x_int[i] = (input_t)val.data;  // float → ap_fixed
    }
}

void copy_output(hls::stream<output_stream_t>& score_out, score_arr_t score_int, bool last) {
    #pragma HLS inline
    for (int i = 0; i < BDT::fn_classes(n_classes); i++) {
        #pragma HLS pipeline
        output_stream_t val;
        val.data = (float)score_int[i];  // ap_fixed → float
        val.keep = -1;                   // all bytes valid
        val.strb = -1;
        val.last = last && (i == BDT::fn_classes(n_classes) - 1);  // tlast on final output
        score_out.write(val);
    }
}

void my_prj_accelerator(
    int N,
    hls::stream<input_stream_t>&  x,
    hls::stream<output_stream_t>& score)
{
    #pragma HLS interface mode=axis port=x     // AXI-Stream input
    #pragma HLS interface mode=axis port=score // AXI-Stream output
    #pragma HLS interface mode=s_axilite port=N      bundle=control
    #pragma HLS interface mode=s_axilite port=return bundle=control

    #pragma HLS dataflow
    for (int n = 0; n < N; n++) {
        input_arr_t x_int;
        score_arr_t score_int;

        #pragma HLS ARRAY_PARTITION variable=x_int complete
        #pragma HLS ARRAY_PARTITION variable=score_int complete

        copy_input(x, x_int);
        bdt.decision_function(x_int, score_int);
        copy_output(score, score_int, n == N-1);  // tlast on last vector
    }
}