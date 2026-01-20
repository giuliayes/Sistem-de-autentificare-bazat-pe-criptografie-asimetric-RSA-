#include "platform.h"
#include "xil_printf.h"
#include "uart.h"
#include "auth.h"

int main(void)
{
    init_platform();

    if (uart_init() != 0) {
        xil_printf("UART init failed\r\n");
        while (1);
    }

    xil_printf("RSA Auth Server ready.\r\n");

    while (1) {
        handle_auth_session();
    }

    cleanup_platform();
    return 0;
}
