// #include <string.h>
// void score(double * input, double * output) {
//     double var0[2];
//     if (input[10] <= 0.5) {
//         if (input[0] <= 0.5) {
//             memcpy(var0, (double[]){0.09738846572361262, 0.9026115342763874}, 2 * sizeof(double));
//         } else {
//             memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
//         }
//     } else {
//         if (input[1] <= 0.5) {
//             memcpy(var0, (double[]){0.7717231222385862, 0.22827687776141384}, 2 * sizeof(double));
//         } else {
//             memcpy(var0, (double[]){0.37279151943462896, 0.627208480565371}, 2 * sizeof(double));
//         }
//     }
//     memcpy(output, var0, 2 * sizeof(double));
// }


// #include <hls_math.h> 

void score(float input[11], bool &output) {

    // directive INTERFACE utilizate pentru a gestiona
    // input-urile si output-urile catre blocul IP printr-un
    // port cu un I/O specific
    #pragma HLS INTERFACE mode=s_axilite port=input 
    #pragma HLS INTERFACE mode=s_axilite port=output
    #pragma HLS INTERFACE mode=s_axilite port=return

    // directiva PIPELINE
    #pragma HLS PIPELINE II=1

    if (input[10] <= 0.5) {
        if (input[0] <= 0.5) {
            output = 1;
        } else {
            output = 0;
        }
    } else {
        if (input[1] <= 0.5) {
            output = 0;
        } else {
            output = 1;
        }
    }
}
