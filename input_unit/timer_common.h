/*
 * timer_common.h
 * Timer header file, contains the common enums for clock division for both
 * 8-bit and 16-bit timers for the ATtiny84 (and possibly other chips)
 * by William Moy
 */

#ifndef TIMER_COMMON_H
#define TIMER_COMMON_H

#define TIMER_CLOCK_MASK    0b00000111
#define DISABLE_TIMER       0b00000000
#define CLOCK_SCALE_1       0b00000001
#define CLOCK_SCALE_8       0b00000010
#define CLOCK_SCALE_64      0b00000011
#define CLOCK_SCALE_256     0b00000100
#define CLOCK_SCALE_1024    0b00000101
#define EXT_CLOCK_FALLING   0b00000110
#define EXT_CLOCK_RISING    0b00000111

#endif
