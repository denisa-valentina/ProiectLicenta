#include <iostream>

#include "myproject.h"
#include "parameters.h"

void myproject(
    input_t input_layer[100],
    result_t layer9_out[1]
) {

    // hls-fpga-machine-learning insert IO
    // #pragma HLS ARRAY_RESHAPE variable=input_layer complete dim=0
    // #pragma HLS ARRAY_PARTITION variable=layer9_out complete dim=0
    // #pragma HLS INTERFACE ap_vld port=input_layer,layer9_out 
    #pragma HLS interface mode=axis port=input_layer
    #pragma HLS interface mode=axis port=layer9_out
    #pragma HLS interface mode=s_axilite port=return bundle=control
    #pragma HLS PIPELINE

    input_t local_input[100];
    #pragma HLS ARRAY_PARTITION variable=local_input complete dim=0
    for (int i = 0; i < n_in; i++) {
        #pragma HLS UNROLL
        local_input[i] = input_layer[i];
    }

    // hls-fpga-machine-learning insert load weights
#ifndef __SYNTHESIS__
    static bool loaded_weights = false;
    if (!loaded_weights) {
        nnet::load_weights_from_txt<model_default_t, 6400>(w2, "w2.txt");
        nnet::load_weights_from_txt<model_default_t, 64>(b2, "b2.txt");
        nnet::load_weights_from_txt<model_default_t, 2048>(w5, "w5.txt");
        nnet::load_weights_from_txt<model_default_t, 32>(b5, "b5.txt");
        nnet::load_weights_from_txt<model_default_t, 32>(w8, "w8.txt");
        nnet::load_weights_from_txt<model_default_t, 1>(b8, "b8.txt");
        loaded_weights = true;    }
#endif
    // ****************************************
    // NETWORK INSTANTIATION
    // ****************************************

    // hls-fpga-machine-learning insert layers

    layer2_t layer2_out[64];
    #pragma HLS ARRAY_PARTITION variable=layer2_out complete dim=0

    layer3_t layer3_out[64];
    #pragma HLS ARRAY_PARTITION variable=layer3_out complete dim=0

    layer5_t layer5_out[32];
    #pragma HLS ARRAY_PARTITION variable=layer5_out complete dim=0

    layer6_t layer6_out[32];
    #pragma HLS ARRAY_PARTITION variable=layer6_out complete dim=0

    layer8_t layer8_out[1];
    #pragma HLS ARRAY_PARTITION variable=layer8_out complete dim=0

    nnet::dense<input_t, layer2_t, config2>(local_input, layer2_out, w2, b2);
    // nnet::dense<input_t, layer2_t, config2>(input_layer, layer2_out, w2, b2); // dense

    nnet::relu<layer2_t, layer3_t, relu_config3>(layer2_out, layer3_out); // dense_relu

    nnet::dense<layer3_t, layer5_t, config5>(layer3_out, layer5_out, w5, b5); // dense_1

    nnet::relu<layer5_t, layer6_t, relu_config6>(layer5_out, layer6_out); // dense_1_relu

    nnet::dense<layer6_t, layer8_t, config8>(layer6_out, layer8_out, w8, b8); // dense_2

    nnet::sigmoid<layer8_t, result_t, sigmoid_config9>(layer8_out, layer9_out); // dense_2_sigmoid

}

