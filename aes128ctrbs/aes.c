#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

typedef struct param {
    uint8_t nonce[12];
    uint8_t ctr[4];
    uint8_t rk[2*11*16];
} param;

extern void AES_128_keyschedule(const uint8_t *, uint8_t *);
extern void AES_128_encrypt_ctr(param const *, const uint8_t *, uint8_t *, uint32_t);
#define AES_128_decrypt_ctr AES_128_encrypt_ctr

#define NUM_BLOCKS    256
#define INPUT_LENGTH  (NUM_BLOCKS*16)
#define OUTPUT_LENGTH (((INPUT_LENGTH+32)/32)*32)

int main(void)
{
    // Test vectors from NIST SP 800-38A F.5.1
    uint8_t nonce[12] = {0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb};
    uint8_t ctr[4]    = {0xfc,0xfd,0xfe,0xff};
    uint8_t key[16]   = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    uint8_t in[INPUT_LENGTH] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
                                0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
                                0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef};
    uint8_t out[OUTPUT_LENGTH];
    param p;
    memcpy(p.ctr, ctr, 4);
    memcpy(p.nonce, nonce, 12);

    // Fill instruction cache and train branch predictors
    AES_128_keyschedule(key, p.rk);
    AES_128_keyschedule(key, p.rk);
    AES_128_keyschedule(key, p.rk);
    AES_128_keyschedule(key, p.rk);
    uint64_t oldcount = getcycles();
    AES_128_keyschedule(key, p.rk);
    uint64_t cyclecount = getcycles()-oldcount;

    // Print all round keys
    unsigned int i, j;
    for (i = 0; i < 2*11*4; ++i) {
        printf("rk[%2d]: ", i);
        for (j = 0; j < 4; ++j) {
            printf("%02x", p.rk[i*4+j]);
        }
        printf("\n");
    }

    printf("cyc: %u\n", (unsigned int)cyclecount);

    // Fill instruction cache and train branch predictors
    AES_128_encrypt_ctr(&p, in, out, INPUT_LENGTH);
    AES_128_encrypt_ctr(&p, in, out, INPUT_LENGTH);
    AES_128_encrypt_ctr(&p, in, out, INPUT_LENGTH);
    AES_128_encrypt_ctr(&p, in, out, INPUT_LENGTH);
    oldcount = getcycles();
    AES_128_encrypt_ctr(&p, in, out, INPUT_LENGTH);
    cyclecount = getcycles()-oldcount;

    printf("cyc: %u\n", (unsigned int)cyclecount);

    // Print ciphertext
    memcpy(p.ctr, ctr, 4);
    AES_128_encrypt_ctr(&p, in, out, INPUT_LENGTH);
    printf("out: ");
    for (i = 0; i < INPUT_LENGTH; ++i) {
        printf("%02x", out[i]);
    }
    printf("\n");

    return 0;
}

