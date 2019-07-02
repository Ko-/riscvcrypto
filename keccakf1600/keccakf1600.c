#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

extern void keccakf1600(uint32_t *lanes);

int main(void)
{
    // Keccak state as bit-interleaved lanes
    uint32_t lanes[50] = {0};

    // Fill instruction cache and train branch predictors
    keccakf1600(lanes);
    keccakf1600(lanes);
    keccakf1600(lanes);
    keccakf1600(lanes);
    keccakf1600(lanes);
    keccakf1600(lanes);
    uint64_t oldcount = getcycles();
    keccakf1600(lanes);
    uint64_t cyclecount = getcycles()-oldcount;

    printf("cyc: %u\n", (unsigned int)cyclecount);

    memset(lanes, 0, 50*sizeof(uint32_t));
    keccakf1600(lanes);

    // Print ciphertext
    printf("out:\n");
    for (unsigned int i = 0; i < 50; ++i) {
        printf("lanes[%2d] = %08lx\n", i, lanes[i]);
    }

    return 0;
}

