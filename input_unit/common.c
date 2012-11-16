/*
 *
 *
 */

#include "common.h"
#include "types.h"

/* Assumes F_cpu of 8MHz */
#define ONE_MS 800 // 8000 cycles / # of instructions per ms loop
void delay_ms(DWORD ms) {
    volatile DWORD vms;
    while (ms > 0) {
        ms--;
        for (vms = ONE_MS; vms > 0; vms--)
            ;
    }
}
