/*
 * timer_common.h
 * Timer header file, contains the common enums for clock division for both
 * 8-bit and 16-bit timers for the ATtiny84 (and possibly other chips)
 * by William Moy
 */

#ifndef TIMER_COMMON_H
#define TIMER_COMMON_H

enum TIMER_CLOCK_SELECT {
	DISABLE_TIMER,
	CLOCK_SCALE_1,
	CLOCK_SCALE_8,
	CLOCK_SCALE_64,
	CLOCK_SCALE_256,
	CLOCK_SCALE_1024,
	EXT_CLOCK_FALLING,
	EXT_CLOCK_RISING
};

#endif
