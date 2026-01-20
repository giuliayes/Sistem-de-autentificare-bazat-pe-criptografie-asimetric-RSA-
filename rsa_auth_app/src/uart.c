// uart.c
#include "uart.h"
#include "xparameters.h"
#include "xuartps.h"

static XUartPs UartPs;

#define UART_BASEADDR  XPAR_XUARTPS_0_BASEADDR   // <- asta e baza UART1
#define UART_BAUDRATE  115200

int uart_init(void)
{
    XUartPs_Config *cfg;
    int status;

    // LookupConfig primeste base address
    cfg = XUartPs_LookupConfig(UART_BASEADDR);
    if (cfg == NULL) {
        return -1;
    }

    status = XUartPs_CfgInitialize(&UartPs, cfg, cfg->BaseAddress);
    if (status != XST_SUCCESS) {
        return -2;
    }

    XUartPs_SetBaudRate(&UartPs, UART_BAUDRATE);
    return 0;
}

void uart_send_bytes(const uint8_t *buf, uint16_t len)
{
    uint16_t sent = 0;
    while (sent < len) {
        int n = XUartPs_Send(&UartPs, (uint8_t *)(buf + sent), len - sent);
        if (n > 0) sent += n;
    }
}

uint16_t uart_recv_bytes(uint8_t *buf, uint16_t len)
{
    uint16_t recvd = 0;
    while (recvd < len) {
        int n = XUartPs_Recv(&UartPs, buf + recvd, len - recvd);
        if (n > 0) recvd += n;
    }
    return recvd;
}
