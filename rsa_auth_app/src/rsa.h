// rsa.h
#pragma once
#include <stdint.h>

typedef struct {
    uint64_t n;
    uint64_t e;
} RsaPublicKey;

uint64_t rsa_modexp(uint64_t base, uint64_t exp, uint64_t mod);
int rsa_verify_challenge(uint64_t challenge, uint64_t signature,
                         const RsaPublicKey *pub);

// public key a clientului (PC)
extern const RsaPublicKey g_client_pubkey;
