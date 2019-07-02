#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

extern void AES_128_keyschedule(const uint8_t *, uint8_t *);
extern void AES_128_keyschedule_dec(const uint8_t *, uint8_t *);
extern void AES_128_encrypt(const uint8_t *, const uint8_t *, uint8_t *);
extern void AES_128_decrypt(const uint8_t *, const uint8_t *, uint8_t *);

int main(void)
{
    const uint8_t key[16] = {4,5,6,7,4,5,6,8,4,5,6,9,4,5,6,10};
    uint8_t in[16] = {0,0,0,0,1,2,3,1,2,4,1,2,5,1,2,6};
    uint8_t out[16];

    uint8_t rk[11*16];

    memcpy(rk, key, 16);

    // Fill instruction cache and train branch predictors
    AES_128_keyschedule(key, rk+16);
    AES_128_keyschedule(key, rk+16);
    AES_128_keyschedule(key, rk+16);
    AES_128_keyschedule(key, rk+16);
    uint64_t oldcount = getcycles();
    AES_128_keyschedule(key, rk+16);
    uint64_t cyclecount = getcycles() - oldcount;


    // Print all round keys
    unsigned int i,j;
    for (i = 0; i < 11*4; ++i) {
        printf("rk[%2d]: ", i);
        for (j = 0; j < 4; ++j) {
            printf("%02x", rk[i*4+j]);
        }
        printf("\n");
    }

    printf("cyc: %u\n", (unsigned int)cyclecount);

    // Fill instruction cache and train branch predictors
    AES_128_encrypt(rk, in, out);
    AES_128_encrypt(rk, in, out);
    AES_128_encrypt(rk, in, out);
    AES_128_encrypt(rk, in, out);
    AES_128_encrypt(rk, in, out);
    AES_128_encrypt(rk, in, out);
    oldcount = getcycles();
    AES_128_encrypt(rk, in, out);
    cyclecount = getcycles()-oldcount;

    printf("cyc: %d\n", (unsigned int)cyclecount);

    // Print ciphertext
    printf("out: ");
    for (i = 0; i < 16; ++i) {
        printf("%02x", out[i]);
    }
    printf("\n");

    return 0;
}

