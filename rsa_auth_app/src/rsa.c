// rsa.c
#include "rsa.h"

// public key from PC
const RsaPublicKey g_client_pubkey = {
    .n = 0xaccd20dde4a5da87ULL,
    .e = 0x0000000000010001ULL   // 0x10001 = 65537
};

static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    uint64_t res = 0;
    a %= mod;

    while (b) {
        if (b & 1) {
            if (res >= mod - a) {
                res = res - (mod - a);  
            } else {
                res += a;
            }
        }

        b >>= 1;

        if (a >= mod - a) {
            a = a - (mod - a);         
        } else {
            a += a;
        }
    }

    return res;
}

uint64_t rsa_modexp(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;

    while (exp) {
        if (exp & 1) {
            result = mul_mod(result, base, mod);
        }
        exp >>= 1;
        if (exp) {
            base = mul_mod(base, base, mod);
        }
    }

    return result;
}

int rsa_verify_challenge(uint64_t challenge, uint64_t signature,
                         const RsaPublicKey *pub)
{
    uint64_t recovered = rsa_modexp(signature, pub->e, pub->n);
    return (recovered == challenge);
}
