#include <stdint.h>
#include <stdio.h>

// UARTlite base address
#define UART_BASE 0x80010000

// UARTlite registers
#define UART_RX_FIFO  (*((volatile uint32_t *)(UART_BASE + 0x00)))
#define UART_TX_FIFO  (*((volatile uint32_t *)(UART_BASE + 0x04)))
#define UART_STAT_REG (*((volatile uint32_t *)(UART_BASE + 0x08)))
#define UART_CTRL_REG (*((volatile uint32_t *)(UART_BASE + 0x0C)))

// Status register bits
#define UART_TX_FIFO_FULL  (1 << 3)
#define UART_TX_FIFO_EMPTY (1 << 2)

void uart_putc(char c) {
    // wait until TX FIFO is not full
    while (UART_STAT_REG & UART_TX_FIFO_FULL);
    UART_TX_FIFO = (uint32_t)c;
}

void uart_puts(const char* str) {
    while (*str) {
        uart_putc(*str++);
    }
}

#define BRAM_BASE 0x80000000

uint32_t *mem_p;

int main() {

    // 1. Working with UartLite
    // // reset TX and RX FIFOs via control register
    // UART_CTRL_REG = 0x03;  // bit0=reset TX FIFO, bit1=reset RX FIFO
    // UART_CTRL_REG = 0x00;  // clear reset

    // uart_puts("Hello from Cortex-A53 on ZCU104!\r\n");
    // uart_puts("RF Accelerator ready.\r\n");

    mem_p = (uint32_t *)BRAM_BASE;

	// Write to block memory
	for (int i = 0; i <= 4; i++)
		*(mem_p+i) = 0xFFFFFFFF;

	// Read from block memory
	for (int i = 0; i <= 4; i++)
		printf("%d\n", (unsigned int)*(mem_p+i));

	return 0;




    while(1);
    return 0;
}