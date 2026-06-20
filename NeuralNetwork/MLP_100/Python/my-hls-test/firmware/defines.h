#ifndef DEFINES_H_
#define DEFINES_H_

#include "ap_fixed.h"
#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "nnet_utils/nnet_types.h"
#include <array>
#include <cstddef>
#include <cstdio>
#include <tuple>
#include <tuple>

static const int n_features = 100;

typedef float accelerator_input_t;
typedef float accelerator_output_t;

typedef hls::axis<accelerator_input_t,0,0,0> input_axi_t;
typedef hls::axis<accelerator_output_t,0,0,0> result_axi_t;

// hls-fpga-machine-learning insert numbers

// hls-fpga-machine-learning insert layer-precision
typedef ap_fixed<32,15> input_t;
typedef ap_fixed<32,15> model_default_t;
typedef ap_fixed<32,15> layer2_t;
typedef ap_uint<1> layer2_index;
typedef ap_fixed<32,15> layer3_t;
typedef ap_fixed<18,8> dense_relu_table_t;
typedef ap_fixed<32,15> layer5_t;
typedef ap_uint<1> layer5_index;
typedef ap_fixed<32,15> layer6_t;
typedef ap_fixed<18,8> dense_1_relu_table_t;
typedef ap_fixed<32,15> layer8_t;
typedef ap_uint<1> layer8_index;
typedef ap_fixed<32,15> result_t;
typedef ap_fixed<18,8> dense_2_sigmoid_table_t;

// hls-fpga-machine-learning insert emulator-defines


#endif
