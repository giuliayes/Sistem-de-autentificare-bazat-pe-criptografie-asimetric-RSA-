// protocol.c
#include "protocol.h"
#include "uart.h"

uint16_t crc16(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

void send_message(uint8_t type, const uint8_t *payload, uint16_t length)
{
    uint8_t header[4];
    header[0] = MSG_START_BYTE;
    header[1] = type;
    header[2] = (length >> 8) & 0xFF;
    header[3] = length & 0xFF;

    uint16_t crc = crc16(&header[1], 3);           // type + len
    crc ^= crc16(payload, length);

    uart_send_bytes(header, 4);
    if (length) uart_send_bytes(payload, length);

    uint8_t crc_bytes[2] = { crc >> 8, crc & 0xFF };
    uart_send_bytes(crc_bytes, 2);
}

int recv_message(Message *msg)
{
    uint8_t b;

    // caut start byte
    do {
        uart_recv_bytes(&b, 1);
    } while (b != MSG_START_BYTE);

    uint8_t hdr[3];
    uart_recv_bytes(hdr, 3);
    msg->type = hdr[0];
    msg->length = ((uint16_t)hdr[1] << 8) | hdr[2];
    if (msg->length > sizeof(msg->payload)) return -1;

    if (msg->length) uart_recv_bytes(msg->payload, msg->length);

    uint8_t crc_raw[2];
    uart_recv_bytes(crc_raw, 2);
    uint16_t crc_recv = ((uint16_t)crc_raw[0] << 8) | crc_raw[1];

    uint8_t t[3] = { msg->type, hdr[1], hdr[2] };
    uint16_t crc = crc16(t, 3);
    crc ^= crc16(msg->payload, msg->length);

    return (crc == crc_recv) ? 0 : -2;
}
