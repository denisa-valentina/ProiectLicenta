#ifndef MYPROJECT_H_
#define MYPROJECT_H_

#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_stream.h"

#include "defines.h"


// Prototype of top level function for C-synthesis
void myproject(
    input_t input_layer[25],
    result_t layer9_out[1]
);

void my_prj_accelerator(
    int N,
    hls::stream<input_axi_t>&  x,
    hls::stream<result_axi_t>& result);
    
// hls-fpga-machine-learning insert emulator-defines


#endif
