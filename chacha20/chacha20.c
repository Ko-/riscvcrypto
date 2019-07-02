#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

extern void chacha20(uint8_t *out, const uint8_t *in, size_t inlen, const uint8_t *key, const uint8_t *nonce, uint32_t ctr);

int main(void)
{
    // https://tools.ietf.org/html/rfc8439#section-2.4.2
    const uint8_t key[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
    const uint8_t nonce[12] = {0,0,0,0,0,0,0,74,0,0,0,0};
    const uint32_t ctr = 1;
    uint8_t in[4096] = "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it.";
    uint8_t out[4096];


    // Fill instruction cache and train branch predictors
    chacha20(out, in, sizeof(in), key, nonce, ctr);
    chacha20(out, in, sizeof(in), key, nonce, ctr);
    chacha20(out, in, sizeof(in), key, nonce, ctr);
    chacha20(out, in, sizeof(in), key, nonce, ctr);
    chacha20(out, in, sizeof(in), key, nonce, ctr);
    chacha20(out, in, sizeof(in), key, nonce, ctr);
    uint64_t oldcount = getcycles();
    chacha20(out, in, sizeof(in), key, nonce, ctr);
    uint64_t cyclecount = getcycles()-oldcount;

    printf("cyc: %u\n", (unsigned int)cyclecount);


    // Print ciphertext
    printf("out: ");
    for (unsigned int i = 0; i < 114; ++i) {
        printf("%02x ", out[i]);
    }
    printf("\n");

    return 0;
}

