#ifndef COMMON_H
#define COMMON_H

#define bit_set1(reg, b1)             ((reg) |= 1 << (b1))
#define bit_set2(reg, b1, b2)         ((reg) |= 1 << (b1) | 1 << (b2))
#define bit_set3(reg, b1, b2, b3)     ((reg) |= 1 << (b1) | 1 << (b2) | 1 << b3)
#define bit_set4(reg, b1, b2, b3, b4) ((reg) |= 1 << (b1) | 1 << (b2) | 1 << (b3) | 1 << (b4))
#define bit_clear1(reg, b1)             ((reg) &= ~(1 << (b1))
#define bit_clear2(reg, b1, b2)         ((reg) &= ~(1 << (b1) | 1 << (b2))
#define bit_clear3(reg, b1, b2, b3)     ((reg) &= ~(1 << (b1) | 1 << (b2) | 1 << (b3))
#define bit_clear4(reg, b1, b2, b3, b4) ((reg) &= ~(1 << (b1) | 1 << (b2) | 1 << (b3) | 1 << (b4))

#endif
