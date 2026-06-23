#include "xaxidma.h"
#include "xparameters.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "sleep.h"
#include "xuartlite.h"
#include "xmy_prj_accelerator.h"
#include "test_data.h"
#include "result_data.h"   // all_labels[], N_LABELS

/*
 * ============================================================
 * Memory map
 * ============================================================
 */
#define MEM_BASE_ADDR       0x10000000
#define TX_BUFFER_BASE      (MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE      (MEM_BASE_ADDR + 0x00300000)

/*
 * ============================================================
 * Dataset dimensions
 * ============================================================
 */
#define N_VECTORS           1
#define N_FEATURES          50

#define TX_BUFFER_SIZE      (N_VECTORS * N_FEATURES * sizeof(float))
#define RX_BUFFER_SIZE      (N_VECTORS * sizeof(float))

#define BYTES_PER_VECTOR    N_FEATURES * sizeof(float)

#define DATA_SIZE_B         (N_VECTORS_DATA * BYTES_PER_VECTOR)
#define CHUNK_SIZE_B        (512 * 1024) // 768KB
#define CHUNK_SIZE_NB_VEC   (CHUNK_SIZE_B / BYTES_PER_VECTOR)  

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
static XAxiDma             AxiDma;
static XUartLite           UartLite;
static XMy_prj_accelerator Accel;

/*
 * ============================================================
 * Single test vector
 * ============================================================
 */
static const float test_vector[N_FEATURES] = {
    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.25631946325302124f,    0.0f,    0.0f,    0.0f,    0.22997435927391052f,    0.0f,    0.0f,    0.115960493683815f,    0.0f,    0.0f,    0.0f,    0.20158948004245758f,    0.0f,    0.6010909676551819f,    0.0f,    0.0f,    0.6660944819450378f,    0.0f,    0.14944817125797272f
};

/*
 * ============================================================
 * UART helpers
 * ============================================================
 */
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
    int len = sprintf(buf, "%s%d.%05d ", neg ? "-" : "", integer, decimal);
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
 * DMA + Accelerator + UART init
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

static int init_dma(void) {
    XAxiDma_Config *cfg;
    int status;

#ifndef SDT
    cfg = XAxiDma_LookupConfig(XPAR_AXIDMA_0_DEVICE_ID);
#else
    cfg = XAxiDma_LookupConfig(XPAR_XAXIDMA_0_BASEADDR);
#endif

    if (!cfg) { xil_printf("DMA config not found\r\n"); return XST_FAILURE; }

    status = XAxiDma_CfgInitialize(&AxiDma, cfg);
    if (status != XST_SUCCESS) { xil_printf("DMA init failed\r\n"); return XST_FAILURE; }

    if (XAxiDma_HasSg(&AxiDma)) { xil_printf("DMA in SG mode\r\n"); return XST_FAILURE; }

    XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
    XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

    xil_printf("DMA initialized\r\n");
    return XST_SUCCESS;
}

static int init_accel(void) {
    int status = XMy_prj_accelerator_Initialize(
        &Accel, XPAR_MY_PRJ_ACCELERATOR_0_BASEADDR);
    if (status != XST_SUCCESS) { xil_printf("Accelerator init failed\r\n"); return XST_FAILURE; }
    xil_printf("Accelerator initialized\r\n");
    return XST_SUCCESS;
}

static int init_uart(void) {
    int status = XUartLite_Initialize(
        &UartLite, XPAR_AXI_UARTLITE_0_BASEADDR);
    if (status != XST_SUCCESS) { xil_printf("UART init failed\r\n"); return XST_FAILURE; }
    xil_printf("UART initialized\r\n");
    return XST_SUCCESS;
}

/*
 * ============================================================
 * Core inference
 * ============================================================
 */
 static int run_inference(int n_vectors) {
    u32 tx_size = n_vectors * N_FEATURES * sizeof(float);
    u32 rx_size = n_vectors * sizeof(float);

    xil_printf("Running inference on %d vectors...\r\n", n_vectors);

    XAxiDma_Reset(&AxiDma);
    while (!XAxiDma_ResetIsDone(&AxiDma));

    u64 freq  = get_timer_freq();
    u64 start = read_cycle_counter();

    XMy_prj_accelerator_Set_N(&Accel, n_vectors);
    Xil_DCacheFlushRange(TX_BUFFER_BASE, tx_size);
    Xil_DCacheFlushRange(RX_BUFFER_BASE, rx_size);
    XMy_prj_accelerator_Start(&Accel);

    XAxiDma_SimpleTransfer(&AxiDma, (UINTPTR)RX_BUFFER_BASE,
        rx_size, XAXIDMA_DEVICE_TO_DMA);
    XAxiDma_SimpleTransfer(&AxiDma, (UINTPTR)TX_BUFFER_BASE,
        tx_size, XAXIDMA_DMA_TO_DEVICE);

    while (XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE));
    while (!XMy_prj_accelerator_IsDone(&Accel));
    while (XAxiDma_Busy(&AxiDma, XAXIDMA_DEVICE_TO_DMA));

    Xil_DCacheInvalidateRange(RX_BUFFER_BASE, rx_size);

    u64 end   = read_cycle_counter();
    u64 ticks = end - start;
    u64 ns    = ticks * 1000000000ULL / freq;
    u32 us    = (u32)(ticks * 1000000ULL / freq);

    xil_printf("Number of ticks: %llu\r\n", ticks);
    xil_printf("Number of ticks per vector: %llu\r\n", ticks / n_vectors);
    xil_printf("Inference time: %d us / %llu ns\r\n", us, ns);
    xil_printf("Per vector:     %llu ns\r\n", ns / n_vectors);

    return XST_SUCCESS;
}
// static int run_inference(int n_vectors) {
//     u32 tx_size; // = n_vectors * N_FEATURES * sizeof(float);
//     u32 rx_size; // = n_vectors * sizeof(float);
//     int i;

//     xil_printf("Running inference on %d vectors...\r\n", n_vectors);

//     XAxiDma_Reset(&AxiDma);
//     while (!XAxiDma_ResetIsDone(&AxiDma));

//     u64 freq  = get_timer_freq();
//     u64 start = read_cycle_counter();

//     for(i = 0; i < n_vectors; i = i + CHUNK_SIZE_NB_VEC) 
//     {
//         u32 current_chunk = (i + CHUNK_SIZE_NB_VEC < n_vectors) ? CHUNK_SIZE_NB_VEC : (n_vectors - i);
//         tx_size = current_chunk * N_FEATURES * sizeof(float);
//         rx_size = current_chunk * sizeof(float);
        
//         // XMy_prj_accelerator_Set_N(&Accel, n_vectors);
        
//         XMy_prj_accelerator_Set_N(&Accel, current_chunk);
//         Xil_DCacheFlushRange(TX_BUFFER_BASE + i * N_FEATURES * sizeof(float), tx_size);
//         Xil_DCacheFlushRange(RX_BUFFER_BASE + i * sizeof(float), rx_size);

//         XAxiDma_SimpleTransfer(&AxiDma, (UINTPTR)(RX_BUFFER_BASE + i * sizeof(float)),
//             rx_size, XAXIDMA_DEVICE_TO_DMA);
//         XAxiDma_SimpleTransfer(&AxiDma, (UINTPTR)(TX_BUFFER_BASE + i * N_FEATURES * sizeof(float)),
//             tx_size, XAXIDMA_DMA_TO_DEVICE);
//         XMy_prj_accelerator_Start(&Accel);

//         while (XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE));
//         while (!XMy_prj_accelerator_IsDone(&Accel));
//         while (XAxiDma_Busy(&AxiDma, XAXIDMA_DEVICE_TO_DMA));

//         Xil_DCacheInvalidateRange(RX_BUFFER_BASE + i * sizeof(float), rx_size);
//     }

//     u64 end   = read_cycle_counter();
//     u64 ticks = end - start;
//     u64 ns    = ticks * 1000000000ULL / freq;
//     u32 us    = (u32)(ticks * 1000000ULL / freq);

//     xil_printf("Number of ticks: %llu\r\n", ticks);
//     xil_printf("Number of ticks per vector: %llu\r\n", ticks / n_vectors);
//     xil_printf("Inference time: %d us / %llu ns\r\n", us, ns);
//     xil_printf("Per vector:     %llu ns\r\n", ns / n_vectors);

//     return XST_SUCCESS;
// }

/*
 * ============================================================
 * Apply threshold:  score < 0.5  →  1,  else  →  0
 * ============================================================
 */
static inline int apply_threshold(float score) {
    return (score <= SCORE_THRESHOLD) ? 1 : 0;
}

/*
 * ============================================================
 * Compute & display metrics folosind all_labels din result_data.h
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

    /* trimite metricile pe o singura linie separata prin spatii:
     * METRICS <accuracy> <precision> <recall> <f1>
     * usor de parsat in Python pe PC */
    uart_send_str("HARDWARE METRICS ");
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

    float *tx = (float *)TX_BUFFER_BASE;
    float *rx = (float *)RX_BUFFER_BASE;

    for (int i = 0; i < N_FEATURES; i++)
        tx[i] = test_vector[i];

    if (run_inference(1) != XST_SUCCESS)
        return XST_FAILURE;

    float score = rx[0];
    int   pred  = apply_threshold(score);

    uart_send_str("Raw score : "); uart_send_float(score);
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

    float *tx = (float *)TX_BUFFER_BASE;
    float *rx = (float *)RX_BUFFER_BASE;

    /* ---- load input vectors ---- */
    xil_printf("Loading %d vectors...\r\n", N_VECTORS_DATA);
    for (int i = 0; i < N_VECTORS_DATA * N_FEATURES; i++)
        tx[i] = all_vectors[i];
    xil_printf("Vectors loaded\r\n");

    /* ---- run inference ---- */
    if (run_inference(N_VECTORS_DATA) != XST_SUCCESS)
        return XST_FAILURE;

    /* ---- apply threshold & collect predictions ---- */
    static int predictions[6184];

    for (int i = 0; i < N_VECTORS_DATA; i++)
        predictions[i] = apply_threshold(rx[i]);

    /* ---- trimite rezultatele thresholdate catre PC ---- */
    // uart_send_str("BEGIN_RESULTS\r\n");
    // for (int i = 0; i < N_VECTORS_DATA; i++) {
    //     uart_send_float(rx[i]);
    // }
    // uart_send_str("END_RESULTS\r\n");

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
    xil_printf("\r\n=== RF Accelerator - ZCU104 ===\r\n");

    if (init_dma()   != XST_SUCCESS) return XST_FAILURE;
    if (init_accel() != XST_SUCCESS) return XST_FAILURE;
    if (init_uart()  != XST_SUCCESS) return XST_FAILURE;

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
    while(1);
    return 0;
}