/*
 * common.c
 * Helper functions
 * by William Moy
 */

#include "common.h"
#include "types.h"

/** 
 * delay_ms
 * Busy-wait for a specified time, not 100% time-accurate.  Assumes 8MHz F_cpu.
 * args:
 *  ms: time to wait in milliseconds
 * returns:
 *  none
 */
#define ONE_MS 800 // 8000 cycles / # of instructions per ms loop
void delay_ms(DWORD ms) {
    volatile DWORD vms;
    while (ms > 0) {
        ms--;
        for (vms = ONE_MS; vms > 0; vms--)
            ;
    }
}

/** 
 * sq16_to_bar8
 * Converts a 16-bit squared value to an 8-bit bar.  Basically imitates a
 * square-root by putting the value in a bin and display that value as a bar.
 * e.g. 0b0010111001100001  -->  0b01111111
 *      0b0000000000101011  -->  0b00000111
 * args:
 *  a: 16-bit squared value
 * returns:
 *  BYTE: 8-bit display-friendly "bar"
 */
BYTE sq16_to_bar8(DWORD a) {
    BYTE val;
    BYTE b = (BYTE)(a >> 8);
    BYTE c = (BYTE)a;
    if (a == 0)
        val = 0x00;
    else if (0xC0 & b)
        val = 0xFF;
    else if (0x30 & b)
        val = 0x7F;
    else if (0x0C & b)
        val = 0x3F;
    else if (0x03 & b)
        val = 0x1F;
    else if (0xC0 & c)
        val = 0x0F;
    else if (0x30 & c)
        val = 0x07;
    else if (0x0C & c)
        val = 0x03;
    else
        val = 0x01;
    return val;
}
