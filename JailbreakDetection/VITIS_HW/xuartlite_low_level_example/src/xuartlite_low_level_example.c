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
#define RX_BUFFER_BASE      (MEM_BASE_ADDR + 0x00200000)

/*
 * ============================================================
 * Dataset dimensions
 * ============================================================
 */
#define N_VECTORS           1
#define N_FEATURES          100

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
static const float test_vector[100] = {
    -3.180260658264160156e-01f, 5.945025682449340820e-01f, 4.507140219211578369e-01f, 1.884206384420394897e-01f,
    -1.103570088744163513e-01f, -7.465211749076843262e-01f, 6.344497203826904297e-01f, 1.063434839248657227e+00f,
    -7.501009106636047363e-01f, -6.918004751205444336e-01f, -5.148951709270477295e-02f, -1.049048662185668945e+00f,
     2.206074148416519165e-01f, -1.710264086723327637e-01f, 4.742447286844253540e-02f, -2.338473498821258545e-01f,
     4.758413732051849365e-01f, -9.752442836761474609e-01f, 1.479357481002807617e-03f, -1.405520677566528320e+00f,
     4.191161692142486572e-01f, 2.354633659124374390e-01f, 6.176264882087707520e-01f, -2.623722553253173828e-01f,
    -8.958978652954101562e-01f, 3.458454608917236328e-01f, -6.208254694938659668e-01f, -2.827559709548950195e-01f,
    -1.040349841117858887e+00f, -1.827840805053710938e-01f, 6.436964869499206543e-01f, 6.201452016830444336e-02f,
     3.060256242752075195e-01f, -7.392284870147705078e-01f, -2.385312020778656006e-01f, 4.735584259033203125e-01f,
     1.233108490705490112e-01f, -4.019258320331573486e-01f, -4.251557886600494385e-01f, -1.049932241439819336e+00f,
     1.161928921937942505e-01f, -5.454487800598144531e-01f, 2.409457862377166748e-01f, -1.841808259487152100e-01f,
     4.347918927669525146e-01f, -2.133195549249649048e-01f, -5.591213703155517578e-03f, -4.206407070159912109e-01f,
     2.959002554416656494e-02f, 4.339279830455780029e-01f, 3.169109225273132324e-01f, -1.440617293119430542e-01f,
     1.686312854290008545e-01f, -1.451938152313232422e-01f, -4.794959127902984619e-01f, 1.890510618686676025e-01f,
     1.271196007728576660e-01f, -2.388508059084415436e-02f, -3.064323365688323975e-01f, -4.041604325175285339e-02f,
     7.929454743862152100e-02f, -1.577727347612380981e-01f, 2.292458713054656982e-01f, -3.957373276352882385e-02f,
    -2.390930801630020142e-01f, 5.172908902168273926e-01f, 1.751434057950973511e-01f, 7.399323582649230957e-01f,
    -7.409241795539855957e-01f, 5.272132158279418945e-01f, -3.278865814208984375e-01f, -1.651272177696228027e-04f,
     7.794157266616821289e-01f, -4.905855283141136169e-02f, 5.835255980491638184e-01f, 4.910364747047424316e-01f,
    -3.274962902069091797e-01f, 2.655715867877006531e-02f, -4.064342975616455078e-01f, -9.298752993345260620e-02f,
    -4.124445319175720215e-01f, -4.399001002311706543e-01f, -7.369785308837890625e-01f, 1.309425830841064453e+00f,
    -3.651550710201263428e-01f, -3.761817216873168945e-01f, 2.537869513034820557e-01f, 5.458538532257080078e-01f,
     7.101207971572875977e-01f, 5.409337878227233887e-01f, 4.831077754497528076e-01f, 3.524691984057426453e-02f,
     1.683949977159500122e-01f, -1.126658320426940918e-01f, 8.431478738784790039e-01f, 5.151598453521728516e-01f,
     3.824149966239929199e-01f, -4.933889508247375488e-01f, 3.890950381755828857e-01f, -6.932591199874877930e-01f
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
    static int predictions[6184];
    int n = (N_VECTORS_DATA <= 6184) ? N_VECTORS_DATA : 6184;

    for (int i = 0; i < n; i++)
        predictions[i] = apply_threshold(rx[i]);

    /* ---- trimite rezultatele thresholdate catre PC ---- */
    // uart_send_str("BEGIN_RESULTS\r\n");
    // for (int i = 0; i < n; i++) {
    //     char buf[8];
    //     int len = sprintf(buf, "%d\r\n", predictions[i]);
    //     XUartLite_Send(&UartLite, (u8*)buf, len);
    //     while (XUartLite_IsSending(&UartLite));
    // }
    // uart_send_str("END_RESULTS\r\n");
    // xil_printf("All %d results sent\r\n", n);

    /* ---- calculeaza si afiseaza metricile ---- */
    compute_and_print_metrics(predictions, n);

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