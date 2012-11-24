/*
 * common.h
 * common header file, includes single-bit set and clear
 * by William Moy
 */

#ifndef COMMON_H
#define COMMON_H

#include "types.h"

// these bit operations take a register to modify (reg) and a bit offset (off)
#define bit_set(reg, off)             ((reg) |= 1 << (off))
#define bit_clear(reg, off)           ((reg) &= ~(1 << (off)))

void delay_ms(DWORD);
BYTE sq16_to_bar8(DWORD);

#endif
