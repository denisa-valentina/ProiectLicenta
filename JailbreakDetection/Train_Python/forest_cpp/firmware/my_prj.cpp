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

// // void copy_input(int n, accelerator_input_t* x_in, input_arr_t x_int){
// //   for(int i = 0; i < n_features; i++){
// //     x_int[i] = x_in[n_features*n + i];
// //   }
// // }

// void copy_input(int n, accelerator_input_t* x_in, input_arr_t x_int){
//   #pragma HLS inline
//   int base = n_features * n;
//   for(int i = 0; i < n_features; i++){
//     # pragma HLS pipeline
//     x_int[i] = (input_t)x_in[base + i]; // casting from float to ap_fixed<32, 18>
//   }
// }

// // void copy_output(int n, score_arr_t score_int, accelerator_output_t* score_out){
// //   for(int i = 0; i < BDT::fn_classes(n_classes); i++){
// //     score_out[BDT::fn_classes(n_classes)*n + i] = score_int[i];
// //   }
// // }

// void copy_output(int n, score_arr_t score_int, accelerator_output_t* score_out){
//   #pragma HLS inline
//   int base = BDT::fn_classes(n_classes)*n;
//   for(int i = 0; i < BDT::fn_classes(n_classes); i++){
//     score_out[base + i] = (float)score_int[i];
//   }
// }

// void my_prj_accelerator(int N, /* int& n_f, int& n_c, */ accelerator_input_t* x, accelerator_output_t* score){
// #pragma HLS interface mode=m_axi port=x offset=slave bundle=gmem0 depth=309200
// #pragma HLS interface mode=m_axi port=score offset=slave bundle=gmem1 depth=6184

// #pragma HLS interface mode=s_axilite port=x bundle=control
// #pragma HLS interface mode=s_axilite port=score bundle=control
// #pragma HLS interface mode=s_axilite port=N bundle=control
// // #pragma HLS interface mode=s_axilite port=n_f bundle=control
// // #pragma HLS interface mode=s_axilite port=n_c bundle=control
// #pragma HLS interface mode=s_axilite port=return bundle=control
// //   n_f = n_features;
// //   n_c = BDT::fn_classes(n_classes);
//   for(int n = 0; n < N; n++){
//     #pragma HLS pipeline
//     input_arr_t x_int;
//     score_arr_t score_int;
//     copy_input(n, x, x_int);
//     bdt.decision_function(x_int, score_int);
//     copy_output(n, score_int, score);
//   }
// }