// protocol.h
#pragma once
#include <stdint.h>

#define MSG_START_BYTE      0xAA

#define MSG_AUTH_REQUEST    0x01
#define MSG_AUTH_CHALLENGE  0x02
#define MSG_AUTH_RESPONSE   0x03
#define MSG_AUTH_RESULT     0x04

typedef struct {
    uint8_t  type;
    uint16_t length;
    uint8_t  payload[64];    // challenge+sig
} Message;

uint16_t crc16(const uint8_t *data, uint16_t len);

void send_message(uint8_t type, const uint8_t *payload, uint16_t length);
int  recv_message(Message *msg);
