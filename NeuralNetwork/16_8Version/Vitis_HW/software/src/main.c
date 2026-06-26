/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

#include <stdio.h>
#include "xil_printf.h"
#include "test_data.h"
#include "xuartlite.h"
#include "mlp_c.h"
#include "result_data.h"

/*
 * ============================================================
 * Memory map
 * ============================================================
 */
#define MEM_BASE_ADDR       0x10000000
#define TX_BUFFER_BASE      (MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE      (MEM_BASE_ADDR + 0x00600000)

/*
 * ============================================================
 * Dataset dimensions
 * ============================================================
 */
#define N_FEATURES          25

#define TX_BUFFER_SIZE      (N_VECTORS * N_FEATURES * sizeof(float))
#define RX_BUFFER_SIZE      (N_VECTORS * sizeof(float))

/*
 * ============================================================
 * Threshold
 * ============================================================
 */
#define SCORE_THRESHOLD     0.5f

/*
 * ============================================================
 * Peripheral instances
 * ============================================================
 */
static XUartLite UartLite;

/*
 * ============================================================
 * Buffers
 * ============================================================
 */
static float test_vector[N_FEATURES] = {
     -0.5469824075698853f,    -1.1784119606018066f,    -0.1156207025051117f,    -0.48589009046554565f,    -0.9483110904693604f,    -0.1390908658504486f,    0.06373749673366547f,    0.9823147654533386f,    -2.1196277141571045f,    -1.418487310409546f,    0.2943456470966339f,    -0.31877198815345764f,    1.4262731075286865f,    1.919734001159668f,    -1.2563890218734741f,    2.0094945430755615f,    0.24952268600463867f,    0.7582359910011292f,    -0.8650880455970764f,    0.9332316517829895f,    -0.48372361063957214f,    0.5980367660522461f,    0.5984618067741394f,    -0.08053360879421234f,    0.9411662220954895f,
};

static float score_res[1];
static float score_res_file[N_VECTORS_DATA];
static int   predictions[N_VECTORS_DATA];   // rezultate dupa thresholding

/*
 * ============================================================
 * UART helpers
 * ============================================================
 */
static int init_uart(void) {
    int status = XUartLite_Initialize(&UartLite, XPAR_AXI_UARTLITE_0_BASEADDR);
    if (status != XST_SUCCESS) {
        xil_printf("UART init failed\r\n");
        return XST_FAILURE;
    }
    xil_printf("UART initialized\r\n");
    return XST_SUCCESS;
}

static void uart_send_str(const char *s) {
    u32 len = 0;
    while (s[len]) len++;
    XUartLite_Send(&UartLite, (u8*)s, len);
    while (XUartLite_IsSending(&UartLite));
}

static void uart_send_float(float val) {
    char buf[32];
    int neg     = (val < 0.0f);
    if (neg) val = -val;
    int integer = (int)val;
    int decimal = (int)((val - (float)integer) * 100000.0f);
    int len = sprintf(buf, "%s%d.%05d\r\n", neg ? "-" : "", integer, decimal);
    XUartLite_Send(&UartLite, (u8*)buf, len);
    while (XUartLite_IsSending(&UartLite));
}

/* float inline — fara newline, pentru metrici pe o singura linie */
static void uart_send_float_inline(float val) {
    char buf[32];
    int neg     = (val < 0.0f);
    if (neg) val = -val;
    int integer = (int)val;
    int decimal = (int)((val - (float)integer) * 100000.0f);
    int len = sprintf(buf, "%s%d.%05d", neg ? "-" : "", integer, decimal);
    XUartLite_Send(&UartLite, (u8*)buf, len);
    while (XUartLite_IsSending(&UartLite));
}

/*
 * ============================================================
 * Cycle counter helpers
 * ============================================================
 */
static inline u64 read_cycle_counter(void) {
    u64 val;
    asm volatile("mrs %0, cntvct_el0" : "=r"(val));
    return val;
}

static inline u64 get_timer_freq(void) {
    u64 freq;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));
    return freq;
}

/*
 * ============================================================
 * Apply threshold:  score < 0.5  →  1,  else  →  0
 * ============================================================
 */
static inline int apply_threshold(float s) {
    return (s > SCORE_THRESHOLD) ? 1 : 0;
}

/*
 * ============================================================
 * Compute & print metrici folosind all_labels din result_data.h
 * Output UART: "METRICS <acc> <prec> <rec> <f1>"
 * ============================================================
 */
static void compute_and_print_metrics(const int *preds, int n) {
    int TP = 0, FP = 0, TN = 0, FN = 0;

    for (int i = 0; i < n; i++) {
        int gt = all_labels[i];
        int pr = preds[i];

        if      (pr == 1 && gt == 1) TP++;
        else if (pr == 1 && gt == 0) FP++;
        else if (pr == 0 && gt == 0) TN++;
        else if (pr == 0 && gt == 1) FN++;
    }

    int total = TP + FP + TN + FN;

    float accuracy  = (total > 0)
                      ? (float)(TP + TN) / total : 0.0f;
    float precision = (TP + FP > 0)
                      ? (float)TP / (TP + FP)    : 0.0f;
    float recall    = (TP + FN > 0)
                      ? (float)TP / (TP + FN)    : 0.0f;
    float f1        = (precision + recall > 0.0f)
                      ? 2.0f * precision * recall / (precision + recall) : 0.0f;

    xil_printf("\r\n========== METRICS ==========\r\n");
    xil_printf("Vectors : %d\r\n", total);
    xil_printf("TP=%d  FP=%d  TN=%d  FN=%d\r\n", TP, FP, TN, FN);

    /* linie usor de parsat in Python:
     * METRICS <accuracy> <precision> <recall> <f1> */
    uart_send_str("METRICS ");
    uart_send_float_inline(accuracy);  uart_send_str(" ");
    uart_send_float_inline(precision); uart_send_str(" ");
    uart_send_float_inline(recall);    uart_send_str(" ");
    uart_send_float_inline(f1);
    uart_send_str("\r\n");

    xil_printf("==============================\r\n");
}

/*
 * ============================================================
 * Phase 1 — single vector sanity check
 * ============================================================
 */
static int phase1_single_vector(void) {
    xil_printf("\r\n--- Phase 1: Single vector test ---\r\n");

    u64 freq  = get_timer_freq();
    u64 start = read_cycle_counter();

    // Call once, before your loop — sets up internal weights/buffers
    mlp_c_initialize();
        
    // input tensor
    k2c_tensor dense_input_input = {
        test_vector,           
        1,                        
        N_FEATURES,                
        {N_FEATURES,1,1,1,1}        
    };

    // output tensor
    float output_data[1];
        k2c_tensor dense_1_output = {
            output_data,
            1,
            1,
            {1,1,1,1,1}
        };

    // Run inference
    mlp_c(&dense_input_input, &dense_1_output);

    // Store result (raw sigmoid output, 0.0–1.0)
    score_res[0] = output_data[0];

    mlp_c_terminate();

    u64 end   = read_cycle_counter();
    u64 ticks = end - start;
    u64 ns    = ticks * 1000000000ULL / freq;
    u32 us    = (u32)(ticks * 1000000ULL / freq);

    xil_printf("Freq: %d\r\n", freq);
    xil_printf("Number of ticks: %d\r\n", ticks);
    xil_printf("Inference time: %d us / %llu ns\r\n", us, ns);

    float s  = score_res[0];
    int pred = apply_threshold(s);

    uart_send_str("Raw score : "); uart_send_float(s);
    xil_printf("Prediction: %d\r\n", pred);

    return XST_SUCCESS;
}

/*
 * ============================================================
 * Phase 2 — full dataset
 * ============================================================
 */
static int phase2_full_dataset(void) {
    xil_printf("\r\n--- Phase 2: Full dataset ---\r\n");
    xil_printf("Running inference on %d vectors...\r\n", N_VECTORS_DATA);

    u64 freq  = get_timer_freq();
    u64 start = read_cycle_counter();

    // Call once, before your loop — sets up internal weights/buffers
    mlp_c_initialize();

    for (int i = 0; i < N_VECTORS_DATA; i++) {
        float *current_vector = &all_vectors[i * N_FEATURES];
        
        // input tensor
        k2c_tensor dense_input_input = {
            current_vector,           
            1,                        
            N_FEATURES,                
            {N_FEATURES,1,1,1,1}        
        };

        // output tensor
        float output_data[1];
            k2c_tensor dense_1_output = {
                output_data,
                1,
                1,
                {1,1,1,1,1}
            };

        // Run inference
        mlp_c(&dense_input_input, &dense_1_output);

        // Store result (raw sigmoid output, 0.0–1.0)
        score_res_file[i] = output_data[0];

    }

    mlp_c_terminate();

    u64 end   = read_cycle_counter();
    u64 ticks = end - start;
    u64 ns    = ticks * 1000000000ULL / freq;
    u32 us    = (u32)(ticks * 1000000ULL / freq);

    xil_printf("Freq: %d\r\n", freq);
    xil_printf("Number of ticks: %llu\r\n", ticks);
    xil_printf("Number of ticks per vector: %llu\r\n", ticks / N_VECTORS_DATA);
    xil_printf("Inference time: %u us / %llu ns\r\n", us, ns);
    xil_printf("Inference time per vector: %llu ns\r\n", ns / N_VECTORS_DATA);

    /* ---- aplica threshold ---- */
    for (int i = 0; i < N_VECTORS_DATA; i++)
        predictions[i] = apply_threshold(score_res_file[i]);

    /* ---- trimite rezultatele catre PC ---- */
    // uart_send_str("BEGIN_RESULTS\r\n");
    // for (int i = 0; i < N_VECTORS_DATA; i++) {
    //     char buf[8];
    //     int len = sprintf(buf, "%d\r\n", predictions[i]);
    //     XUartLite_Send(&UartLite, (u8*)buf, len);
    //     while (XUartLite_IsSending(&UartLite));
    // }
    // uart_send_str("END_RESULTS\r\n");
    // xil_printf("All %d results sent\r\n", N_VECTORS_DATA);

    /* ---- calculeaza si afiseaza metricile ---- */
    compute_and_print_metrics(predictions, N_VECTORS_DATA);

    return XST_SUCCESS;
}

/*
 * ============================================================
 * Main
 * ============================================================
 */
int main(void) {

    xil_printf("\r\n=== RF Software - ZCU104 ===\r\n");

    if (init_uart() != XST_SUCCESS) return XST_FAILURE;

    if (phase1_single_vector() != XST_SUCCESS) {
        xil_printf("Phase 1 FAILED\r\n");
        return XST_FAILURE;
    }
    xil_printf("Phase 1 PASSED\r\n");

    if (phase2_full_dataset() != XST_SUCCESS) {
        xil_printf("Phase 2 FAILED\r\n");
        return XST_FAILURE;
    }
    xil_printf("Phase 2 PASSED\r\n");

    xil_printf("\r\n=== Complete ===\r\n");

    return 0;
}