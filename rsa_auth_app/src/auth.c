// auth.c
#include "auth.h"
#include "protocol.h"
#include "rsa.h"
#include "xil_printf.h"

static uint64_t g_challenge_counter = 1;

static uint64_t generate_challenge(void)
{
    // creste un contor
    return g_challenge_counter++;
}

void handle_auth_session(void)
{
    Message msg;
    int st;

    xil_printf("Waiting for AUTH_REQUEST...\r\n");

    st = recv_message(&msg);
    if (st != 0 || msg.type != MSG_AUTH_REQUEST) {
        xil_printf("Bad AUTH_REQUEST (st=%d, type=%d)\r\n", st, msg.type);
        return;
    }

    xil_printf("AUTH_REQUEST received.\r\n");

    // trimite challenge
    uint64_t challenge = generate_challenge();
    uint8_t chall_bytes[8];
    for (int i = 0; i < 8; i++)
        chall_bytes[7 - i] = (challenge >> (8 * i)) & 0xFF;

    send_message(MSG_AUTH_CHALLENGE, chall_bytes, 8);
    xil_printf("CHALLENGE: 0x%08x%08x\r\n",
               (uint32_t)(challenge >> 32), (uint32_t)challenge);

    // primeste raspuns
    st = recv_message(&msg);
    if (st != 0 || msg.type != MSG_AUTH_RESPONSE || msg.length != 8) {
        xil_printf("Bad AUTH_RESPONSE (st=%d, len=%d)\r\n", st, msg.length);
        return;
    }

    uint64_t signature = 0;
    for (int i = 0; i < 8; i++)
        signature = (signature << 8) | msg.payload[i];

    xil_printf("Signature: 0x%08x%08x\r\n",
               (uint32_t)(signature >> 32), (uint32_t)signature);

    int ok = rsa_verify_challenge(challenge, signature, &g_client_pubkey);
    uint8_t res = ok ? 1 : 0;
    send_message(MSG_AUTH_RESULT, &res, 1);

    xil_printf("AUTH %s\r\n", ok ? "OK" : "FAIL");
}
