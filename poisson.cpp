
#include "includes/header.h"

int random_int_poisson() {
    static unsigned int lfsr = 56789123 & 0x7fffffff;
    unsigned int bit;
    bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
    lfsr = (lfsr >> 1) | (bit << 31);
    return lfsr & 0x7fffffff;
}

void poisson(im image[Sources],
              int spike_reg[Sources],
              int intensity,
              int *spiked) {


#pragma HLS INLINE off
    POISSON: for (int spiker = 0; spiker < Sources; spiker++) {
#pragma HLS PIPELINE
        if ((image[spiker] * intensity * 8421) > random_int_poisson()) {
            spike_reg[*spiked] = spiker;
            (*spiked)++;
        }
    }
}
