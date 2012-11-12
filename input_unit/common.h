#ifndef COMMON_H
#define COMMON_H

#define bit_set(reg, off)             ((reg) |= 1 << (off))
#define bit_clear(reg, off)           ((reg) &= ~(1 << (off)))

#endif
