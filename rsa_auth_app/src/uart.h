// uart.h
#pragma once
#include <stdint.h>
#include "xuartps.h"

int  uart_init(void);
void uart_send_bytes(const uint8_t *buf, uint16_t len);
uint16_t uart_recv_bytes(uint8_t *buf, uint16_t len);
