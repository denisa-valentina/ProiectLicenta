#ifndef DEFINES_H_
#define DEFINES_H_

#include "ap_fixed.h"
#include "ap_int.h"
#include "nnet_utils/nnet_types.h"
#include <array>
#include <cstddef>
#include <cstdio>
#include <tuple>
#include <tuple>


// hls-fpga-machine-learning insert numbers

// hls-fpga-machine-learning insert layer-precision
typedef ap_fixed<32,18> input_t;
typedef ap_fixed<32,18> model_default_t;
typedef ap_fixed<32,18> layer2_t;
typedef ap_uint<1> layer2_index;
typedef ap_fixed<32,18> layer3_t;
typedef ap_fixed<18,8> dense_relu_table_t;
typedef ap_fixed<32,18> layer5_t;
typedef ap_uint<1> layer5_index;
typedef ap_fixed<32,18> layer6_t;
typedef ap_fixed<18,8> dense_1_relu_table_t;
typedef ap_fixed<32,18> layer8_t;
typedef ap_uint<1> layer8_index;
typedef ap_fixed<32,18> result_t;
typedef ap_fixed<18,8> dense_2_sigmoid_table_t;

// hls-fpga-machine-learning insert emulator-defines


#endif
