/*
 *
 *
 */

#include "common.h"
#include "types.h"

/* Assumes F_cpu of 8MHz */
void delay_ms(BYTE ms) {
    volatile BYTE a,b,c;
    for (a=0; a<32; a++) {
        for (b=0; b<250; b++) {
            for (c=0; c<ms; c++) {
                ;
            }
        }
    }
}
