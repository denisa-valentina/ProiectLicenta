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
#define RX_BUFFER_BASE      (MEM_BASE_ADDR + 0x00600000)

/*
 * ============================================================
 * Dataset dimensions
 * ============================================================
 */
#define N_VECTORS           1
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
static XAxiDma             AxiDma;
static XUartLite           UartLite;
static XMy_prj_accelerator Accel;

/*
 * ============================================================
 * Single test vector
 * ============================================================
 */
static const float test_vector[N_FEATURES] = {
    -0.6269310712814331f,    0.7673266530036926f,    0.2686030864715576f,    0.17980369925498962f,    -0.2256859838962555f,    -0.7588006854057312f,    0.6556829810142517f,    0.7482870817184448f,    -0.7567614912986755f,    -0.3753510117530823f,    0.026554260402917862f,    -1.0003787279129028f,    0.10422075539827347f,    -0.17204231023788452f,    0.03581993281841278f,    -0.4132457971572876f,    0.37646958231925964f,    -0.8084053993225098f,    -0.18242639303207397f,    -1.399141788482666f,    0.13798290491104126f,    0.23725198209285736f,    0.5656598806381226f,    -0.06864283978939056f,    -0.5446228981018066f,    0.472150981426239f,    -0.7395725846290588f,    -0.33531370759010315f,    -0.9265177249908447f,    0.1078406348824501f,    0.6604524850845337f,    0.27525582909584045f,    0.3371751308441162f,    -0.5041280388832092f,    -0.06157343089580536f,    0.3459833860397339f,    0.2810601592063904f,    -0.15839783847332f,    -0.32713907957077026f,    -1.0199346542358398f,    0.19232885539531708f,    -0.6812472343444824f,    0.2772009074687958f,    -0.12301234900951385f,    0.5070129036903381f,    -0.32675012946128845f,    0.03502530977129936f,    -0.42008042335510254f,    0.23053774237632751f,    0.24662217497825623f,    0.5057488679885864f,    -0.3786075711250305f,    -0.13488751649856567f,    0.21179571747779846f,    -0.298674613237381f,    0.4832359552383423f,    0.36479368805885315f,    0.21168652176856995f,    -0.7641335725784302f,    -0.07171604037284851f,    0.3034699559211731f,    0.06254972517490387f,    0.16728989779949188f,    0.028246013447642326f,    -0.4354727268218994f,    0.2565627992153168f,    0.2627961039543152f,    0.7226906418800354f,    -0.9885326623916626f,    0.5736985802650452f,    -0.13213270902633667f,    0.16989001631736755f,    0.8655439615249634f,    -0.26354631781578064f,    0.7601479887962341f,    0.43904930353164673f,    -0.19831855595111847f,    0.0591716393828392f,    -0.2795696556568146f,    -0.15257887542247772f,    -0.6851140260696411f,    -0.3212761878967285f,    -0.6899304389953613f,    1.3901020288467407f,    -0.10672765970230103f,    -0.29133155941963196f,    0.296337753534317f,    0.8348475098609924f,    0.9356133937835693f,    0.5409965515136719f,    0.4632737636566162f,    0.035832472145557404f,    0.3177512288093567f,    0.1989912986755371f,    0.7993931770324707f,    0.659734308719635f,    0.44905227422714233f,    -0.5678672194480896f,    0.11290539801120758f,    -0.5856857299804688f
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

/*
 * ============================================================
 * Apply threshold:  score < 0.5  →  1,  else  →  0
 * ============================================================
 */
static inline int apply_threshold(float score) {
    return (score > SCORE_THRESHOLD) ? 1 : 0;
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
    static int predictions[N_VECTORS_DATA];

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