/*
* timer.c
* William Moy
* Timer interface for 8-bit ATtiny AVR
* Designed on ATtiny84
*
*/

#include "timer8.h"
#include "types.h"
#include "common.h"
#include <avr/io.h>
#include <avr/sfr_defs.h>

static volatile BYTE clockOption = DISABLE_TIMER;

void timer8_init(DWORD countTo, BYTE clockSelect) {

    // Set Clear Timer on Compare (CTC) Mode
    bit_clear(TCCR0A, WGM00);
    bit_set(TCCR0A, WGM01);
    bit_clear(TCCR0B, WGM02);

    // Set the upper counter bound
    OCR0A = countTo;

    clockOption = clockSelect;

}

void timer8_start(void) {

    // Reset the ticks
    TCNT0 = 0;

    // Set the prescaler and start the timer
    switch (clockOption) {
		case DISABLE_TIMER:
			bit_clear(TCCR0B, CS00);
            bit_clear(TCCR0B, CS01);
            bit_clear(TCCR0B, CS02);
		break;
		case CLOCK_SCALE_1:
			bit_set(TCCR0B, CS00);
            bit_clear(TCCR0B, CS01);
            bit_clear(TCCR0B, CS02);
		break;
		case CLOCK_SCALE_8:
			bit_clear(TCCR0B, CS00);
            bit_set(TCCR0B, CS01);
            bit_clear(TCCR0B, CS02);
		break;
		case CLOCK_SCALE_64:
			bit_set(TCCR0B, CS00);
            bit_set(TCCR0B, CS01);
            bit_clear(TCCR0B, CS02);
		break;
		case CLOCK_SCALE_256:
			bit_clear(TCCR0B, CS00);
            bit_clear(TCCR0B, CS01);
            bit_set(TCCR0B, CS02);
		break;
		case CLOCK_SCALE_1024:
			bit_set(TCCR0B, CS00);
            bit_clear(TCCR0B, CS01);
            bit_set(TCCR0B, CS02);
		break;
		case EXT_CLOCK_FALLING:
			bit_clear(TCCR0B, CS00);
            bit_set(TCCR0B, CS01);
            bit_set(TCCR0B, CS02);
		break;
		case EXT_CLOCK_RISING:
			bit_set(TCCR0B, CS00);
            bit_set(TCCR0B, CS01);
            bit_set(TCCR0B, CS02);
		break;
		default:
			bit_clear(TCCR0B, CS00);
            bit_clear(TCCR0B, CS01);
            bit_clear(TCCR0B, CS02);
			return;
		break;
	}

    
    
}

void timer8_stop(void) {
	bit_clear(TCCR0B, CS00);
    bit_clear(TCCR0B, CS01);
    bit_clear(TCCR0B, CS02);
}

BYTE timer8_is_tripped(void) {
    return bit_is_set(TIFR0, OCF0A);
}

BYTE timer8_is_running(void) {
    return TCCR0B & (_BV(CS00) | _BV(CS01) | _BV(CS02));
}

void timer8_clear_flag(void) {
    bit_set(TIFR0, OCF0A);
}

DWORD timer8_get_count(void) {
	return TCNT0;
}

void timer8_enable_int(void) {
    bit_set(TIMSK0, OCIE0A);
}

void timer8_disable_int(void) {
    bit_clear(TIMSK0, OCIE0A);
}
