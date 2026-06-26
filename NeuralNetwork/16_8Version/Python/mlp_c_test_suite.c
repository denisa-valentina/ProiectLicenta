#include <stdio.h> 
#include <math.h> 
#include <time.h> 
#include "./include/k2c_include.h" 
#include "mlp_c.h" 

float maxabs(k2c_tensor *tensor1, k2c_tensor *tensor2);
struct timeval GetTimeStamp(); 
 
float test1_dense_input_input_array[25] = {
+1.35811238e+00f,-1.68696771e+00f,+8.43917764e-01f,-1.37130303e+00f,-2.77748898e-01f,
+1.35277916e+00f,+1.91049218e+00f,+1.20091311e-01f,-1.49420948e-01f,+1.06965181e+00f,
-5.97356438e-01f,-1.67417222e+00f,-3.44365926e-01f,+5.82205766e-01f,-1.89255367e+00f,
-4.51507639e-01f,+7.39277221e-02f,-9.21748666e-01f,+1.22041408e-01f,+1.10476120e+00f,
-1.52037820e+00f,+1.69097394e+00f,-1.14712425e+00f,-1.78269029e-01f,+6.19913507e-01f,
}; 
k2c_tensor test1_dense_input_input = { &test1_dense_input_input_array[0], 1, 25, { 25, 1, 1, 1, 1 } }; 
float keras_dense_2_test1_array[1] = {
+8.65667939e-01f,}; 
k2c_tensor keras_dense_2_test1 = { &keras_dense_2_test1_array[0], 1, 1, { 1,1,1,1,1 } }; 
float c_dense_2_test1_array[1] = {0}; 
k2c_tensor c_dense_2_test1 = { &c_dense_2_test1_array[0], 1, 1, { 1,1,1,1,1 } }; 
float test2_dense_input_input_array[25] = {
+1.48227292e+00f,-9.61376180e-01f,+1.76201573e+00f,+1.14909792e+00f,-3.40343225e-01f,
+2.50186246e-01f,-5.61171935e-01f,-1.10595429e-01f,-1.36510436e+00f,-4.84377977e-01f,
+6.86223946e-01f,+1.80585611e+00f,+1.73297242e+00f,-1.91633020e+00f,+1.14838661e+00f,
-1.01047795e+00f,+9.85328113e-01f,-8.93294178e-01f,+1.33739813e+00f,+1.00997539e+00f,
+1.86001799e+00f,-1.76803723e+00f,-2.73832049e-01f,+5.50911230e-01f,-1.62864452e+00f,
}; 
k2c_tensor test2_dense_input_input = { &test2_dense_input_input_array[0], 1, 25, { 25, 1, 1, 1, 1 } }; 
float keras_dense_2_test2_array[1] = {
+4.13370460e-01f,}; 
k2c_tensor keras_dense_2_test2 = { &keras_dense_2_test2_array[0], 1, 1, { 1,1,1,1,1 } }; 
float c_dense_2_test2_array[1] = {0}; 
k2c_tensor c_dense_2_test2 = { &c_dense_2_test2_array[0], 1, 1, { 1,1,1,1,1 } }; 
float test3_dense_input_input_array[25] = {
-1.53750255e+00f,+3.50941201e-01f,+1.23869132e+00f,+2.15210393e-01f,+1.85643679e+00f,
+1.73983273e+00f,-1.54807473e+00f,+8.59089138e-01f,+1.76941240e+00f,+1.66660544e+00f,
-9.12662135e-01f,-1.63451848e+00f,-6.68196463e-01f,+7.54036743e-01f,-1.06972006e+00f,
+8.93287115e-02f,+1.08636739e+00f,-1.69199523e+00f,+6.01015103e-01f,-4.00059181e-01f,
-1.55898208e+00f,+2.35168517e-02f,-7.49713445e-01f,+1.69926490e+00f,-9.59204352e-01f,
}; 
k2c_tensor test3_dense_input_input = { &test3_dense_input_input_array[0], 1, 25, { 25, 1, 1, 1, 1 } }; 
float keras_dense_2_test3_array[1] = {
+3.32136810e-01f,}; 
k2c_tensor keras_dense_2_test3 = { &keras_dense_2_test3_array[0], 1, 1, { 1,1,1,1,1 } }; 
float c_dense_2_test3_array[1] = {0}; 
k2c_tensor c_dense_2_test3 = { &c_dense_2_test3_array[0], 1, 1, { 1,1,1,1,1 } }; 
float test4_dense_input_input_array[25] = {
+7.74781603e-01f,+1.66849335e+00f,-7.73613105e-01f,-3.46572696e-01f,+1.36035054e+00f,
+1.99373375e+00f,+8.14985859e-01f,+1.92417949e+00f,+1.23803067e+00f,-1.17686048e+00f,
+8.35937464e-01f,-1.43773536e+00f,+7.99626991e-01f,-1.77715129e+00f,+1.53739646e+00f,
+1.00742787e+00f,+1.40861048e+00f,+7.41137230e-01f,+9.35479307e-01f,-5.08814542e-01f,
+5.38147810e-01f,+1.11673931e+00f,-1.39922830e+00f,+5.82775338e-01f,+2.54080009e-01f,
}; 
k2c_tensor test4_dense_input_input = { &test4_dense_input_input_array[0], 1, 25, { 25, 1, 1, 1, 1 } }; 
float keras_dense_2_test4_array[1] = {
+1.73734814e-01f,}; 
k2c_tensor keras_dense_2_test4 = { &keras_dense_2_test4_array[0], 1, 1, { 1,1,1,1,1 } }; 
float c_dense_2_test4_array[1] = {0}; 
k2c_tensor c_dense_2_test4 = { &c_dense_2_test4_array[0], 1, 1, { 1,1,1,1,1 } }; 
float test5_dense_input_input_array[25] = {
-1.01735013e+00f,-3.00081751e-01f,-1.03984846e+00f,+1.11868911e-01f,+2.40930275e-01f,
-1.07430742e+00f,+7.38447318e-02f,-1.13116635e+00f,-1.49362257e+00f,+3.05495696e-01f,
+1.51572038e+00f,+5.32634220e-01f,+8.91706605e-01f,-9.70138239e-01f,+8.58059403e-01f,
+1.65172734e-01f,-1.59918908e+00f,+9.88413282e-01f,+1.29813401e+00f,+1.45514120e+00f,
+1.84888747e+00f,+1.04034687e+00f,-1.20754255e+00f,-1.00746762e+00f,-1.14967661e+00f,
}; 
k2c_tensor test5_dense_input_input = { &test5_dense_input_input_array[0], 1, 25, { 25, 1, 1, 1, 1 } }; 
float keras_dense_2_test5_array[1] = {
+5.50795972e-01f,}; 
k2c_tensor keras_dense_2_test5 = { &keras_dense_2_test5_array[0], 1, 1, { 1,1,1,1,1 } }; 
float c_dense_2_test5_array[1] = {0}; 
k2c_tensor c_dense_2_test5 = { &c_dense_2_test5_array[0], 1, 1, { 1,1,1,1,1 } }; 
float test6_dense_input_input_array[25] = {
+4.73600597e-01f,-4.79420244e-01f,-6.55145623e-01f,-1.53631907e+00f,-1.05055187e+00f,
-6.30685019e-01f,-1.08996414e+00f,-1.27622123e+00f,+1.99693602e+00f,+3.70200856e-01f,
+4.36717415e-01f,-8.35023756e-01f,+1.15598083e+00f,+1.86864483e+00f,-3.45352854e-01f,
-7.88557973e-01f,-2.29490574e-01f,+2.73893741e-02f,+1.06257741e+00f,+1.52690822e+00f,
+7.15388646e-01f,-3.72659275e-02f,+2.47245378e-02f,+1.17854796e+00f,+1.95515897e-02f,
}; 
k2c_tensor test6_dense_input_input = { &test6_dense_input_input_array[0], 1, 25, { 25, 1, 1, 1, 1 } }; 
float keras_dense_2_test6_array[1] = {
+5.31045020e-01f,}; 
k2c_tensor keras_dense_2_test6 = { &keras_dense_2_test6_array[0], 1, 1, { 1,1,1,1,1 } }; 
float c_dense_2_test6_array[1] = {0}; 
k2c_tensor c_dense_2_test6 = { &c_dense_2_test6_array[0], 1, 1, { 1,1,1,1,1 } }; 
float test7_dense_input_input_array[25] = {
+4.67486434e-01f,-1.57321882e+00f,-9.64797085e-01f,+1.57298459e+00f,-1.67746531e+00f,
-1.91714461e+00f,+1.89844409e+00f,-1.57461997e-01f,+1.35179457e+00f,+6.21876123e-01f,
+1.49520681e+00f,-1.73959087e+00f,+1.53744466e-01f,-1.20322755e+00f,-1.44813307e+00f,
+9.33499465e-01f,-1.80433498e+00f,-1.58492628e+00f,-1.63472172e-01f,-4.80997723e-01f,
+1.83847383e+00f,-1.83832332e+00f,+6.92022981e-01f,-8.13372423e-01f,-1.75497861e+00f,
}; 
k2c_tensor test7_dense_input_input = { &test7_dense_input_input_array[0], 1, 25, { 25, 1, 1, 1, 1 } }; 
float keras_dense_2_test7_array[1] = {
+7.16799200e-02f,}; 
k2c_tensor keras_dense_2_test7 = { &keras_dense_2_test7_array[0], 1, 1, { 1,1,1,1,1 } }; 
float c_dense_2_test7_array[1] = {0}; 
k2c_tensor c_dense_2_test7 = { &c_dense_2_test7_array[0], 1, 1, { 1,1,1,1,1 } }; 
float test8_dense_input_input_array[25] = {
+1.57260861e+00f,-8.59395602e-01f,+1.58430064e+00f,+1.78131239e+00f,-1.62440605e+00f,
+3.23508909e-01f,-1.18816383e+00f,+1.71956011e+00f,+1.77300803e+00f,-2.17984879e-01f,
+1.81883229e+00f,-1.19880166e+00f,+1.37374522e+00f,+1.43207282e+00f,+2.61357119e-01f,
-8.25016550e-01f,+1.54072966e+00f,+8.68381647e-01f,+8.65235908e-02f,+1.72655810e-01f,
+9.46913421e-01f,-1.27699552e+00f,+1.59902339e+00f,-2.03488891e-01f,+1.31489815e+00f,
}; 
k2c_tensor test8_dense_input_input = { &test8_dense_input_input_array[0], 1, 25, { 25, 1, 1, 1, 1 } }; 
float keras_dense_2_test8_array[1] = {
+5.08809149e-01f,}; 
k2c_tensor keras_dense_2_test8 = { &keras_dense_2_test8_array[0], 1, 1, { 1,1,1,1,1 } }; 
float c_dense_2_test8_array[1] = {0}; 
k2c_tensor c_dense_2_test8 = { &c_dense_2_test8_array[0], 1, 1, { 1,1,1,1,1 } }; 
float test9_dense_input_input_array[25] = {
-1.30206585e+00f,+4.34884052e-01f,-6.49097482e-03f,+3.64518652e-01f,+9.38173019e-01f,
+1.91914623e+00f,-7.64268195e-01f,+6.96040001e-01f,+7.96855351e-01f,+6.03172741e-01f,
-1.69880456e+00f,-1.94124124e-01f,-5.81455834e-01f,+1.88878530e+00f,-7.14019458e-02f,
-1.30764568e+00f,-1.22222690e+00f,-1.78406615e+00f,+4.32620773e-01f,-5.76920406e-01f,
+1.17632616e+00f,+1.75701884e+00f,+1.25128360e+00f,-1.99866172e+00f,+1.28150671e+00f,
}; 
k2c_tensor test9_dense_input_input = { &test9_dense_input_input_array[0], 1, 25, { 25, 1, 1, 1, 1 } }; 
float keras_dense_2_test9_array[1] = {
+8.48048091e-01f,}; 
k2c_tensor keras_dense_2_test9 = { &keras_dense_2_test9_array[0], 1, 1, { 1,1,1,1,1 } }; 
float c_dense_2_test9_array[1] = {0}; 
k2c_tensor c_dense_2_test9 = { &c_dense_2_test9_array[0], 1, 1, { 1,1,1,1,1 } }; 
float test10_dense_input_input_array[25] = {
+7.95157496e-01f,-2.62227194e-01f,-5.60576735e-03f,-1.21740426e+00f,+2.28639908e-01f,
-1.36991999e+00f,-1.42591879e+00f,-1.60600229e+00f,-1.70193431e+00f,+1.01902027e+00f,
-4.74683460e-01f,-1.79471671e+00f,-1.66121113e+00f,+1.22435394e+00f,-5.87624395e-01f,
-1.87232367e+00f,-1.10933811e+00f,+6.12602390e-01f,+1.66084181e+00f,-1.24417312e+00f,
-1.68542825e+00f,-1.60150131e+00f,-6.38047049e-01f,+6.50878656e-01f,-7.40172965e-01f,
}; 
k2c_tensor test10_dense_input_input = { &test10_dense_input_input_array[0], 1, 25, { 25, 1, 1, 1, 1 } }; 
float keras_dense_2_test10_array[1] = {
+6.06482923e-01f,}; 
k2c_tensor keras_dense_2_test10 = { &keras_dense_2_test10_array[0], 1, 1, { 1,1,1,1,1 } }; 
float c_dense_2_test10_array[1] = {0}; 
k2c_tensor c_dense_2_test10 = { &c_dense_2_test10_array[0], 1, 1, { 1,1,1,1,1 } }; 
int main(){
 float errors[10];
 size_t num_tests = 10; 
size_t num_outputs = 1; 
mlp_c_initialize(); 
clock_t t0 = clock(); 
mlp_c(&test1_dense_input_input,&c_dense_2_test1); 
mlp_c(&test2_dense_input_input,&c_dense_2_test2); 
mlp_c(&test3_dense_input_input,&c_dense_2_test3); 
mlp_c(&test4_dense_input_input,&c_dense_2_test4); 
mlp_c(&test5_dense_input_input,&c_dense_2_test5); 
mlp_c(&test6_dense_input_input,&c_dense_2_test6); 
mlp_c(&test7_dense_input_input,&c_dense_2_test7); 
mlp_c(&test8_dense_input_input,&c_dense_2_test8); 
mlp_c(&test9_dense_input_input,&c_dense_2_test9); 
mlp_c(&test10_dense_input_input,&c_dense_2_test10); 

clock_t t1 = clock(); 
printf("Average time over 10 tests: %e s \n",
        ((double)t1-t0)/(double)CLOCKS_PER_SEC/(double)10); 
errors[0] = maxabs(&keras_dense_2_test1,&c_dense_2_test1); 
errors[1] = maxabs(&keras_dense_2_test2,&c_dense_2_test2); 
errors[2] = maxabs(&keras_dense_2_test3,&c_dense_2_test3); 
errors[3] = maxabs(&keras_dense_2_test4,&c_dense_2_test4); 
errors[4] = maxabs(&keras_dense_2_test5,&c_dense_2_test5); 
errors[5] = maxabs(&keras_dense_2_test6,&c_dense_2_test6); 
errors[6] = maxabs(&keras_dense_2_test7,&c_dense_2_test7); 
errors[7] = maxabs(&keras_dense_2_test8,&c_dense_2_test8); 
errors[8] = maxabs(&keras_dense_2_test9,&c_dense_2_test9); 
errors[9] = maxabs(&keras_dense_2_test10,&c_dense_2_test10); 
float maxerror = errors[0]; 
for(size_t i=1; i< num_tests*num_outputs;i++){ 
if (errors[i] > maxerror) { 
maxerror = errors[i];}} 
printf("Max absolute error for 10 tests: %e \n", maxerror);
mlp_c_terminate(); 
if (maxerror > 1e-05) { 
return 1;} 
return 0;
} 

float maxabs(k2c_tensor *tensor1, k2c_tensor *tensor2){ 

    float x = 0; 

    float y = 0; 

    for(size_t i=0; i<tensor1->numel; i++){

    y = fabsf(tensor1->array[i]-tensor2->array[i]);
    if (y>x) {x=y;}}
    return x;}

