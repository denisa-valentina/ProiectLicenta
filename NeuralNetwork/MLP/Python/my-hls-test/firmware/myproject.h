#ifndef MYPROJECT_H_
#define MYPROJECT_H_

#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_stream.h"

#include "defines.h"

// define stream types
// typedef hls::axis<accelerator_input_t, 0, 0, 0>  input_stream_t;
// typedef hls::axis<accelerator_output_t, 0, 0, 0> output_stream_t;

// Prototype of top level function for C-synthesis
void myproject(
    input_t input_layer[100],
    result_t layer9_out[1]
);

// void myproject(
//     // int N,
//     hls::stream<input_stream_t>&  input_layer,
//     hls::stream<output_stream_t>& layer9_out
// );

// hls-fpga-machine-learning insert emulator-defines

#endif
