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

/* Converts a 16-bit squared value to an 8-bit bar */
BYTE sq16_to_bar8(DWORD a) {
    if (a == 0)
        return 0x00;
    if (0xC000 & a)
        return 0xFF;
    if (0x3000 & a)
        return 0x7F;
    if (0x0C00 & a)
        return 0x3F;
    if (0x0300 & a)
        return 0x1F;
    if (0x00C0 & a)
        return 0x0F;
    if (0x0030 & a)
        return 0x07;
    if (0x000C & a)
        return 0x03;
    return 0x01;
}
