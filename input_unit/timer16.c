/*
* timer.c
* William Moy
* Timer interface for 8-bit ATtiny AVR
* Designed on ATtiny84
*
*/

#include "timer16.h"
#include "types.h"
#include "common.h"
#include <avr/io.h>
#include <avr/sfr_defs.h>

static volatile BYTE clockOption = DISABLE_TIMER;

void timer16_init(DWORD countTo, BYTE clockSelect) {

    // Set Clear Timer on Compare (CTC) Mode
    bit_clear(TCCR1A, WGM10);
    bit_set(TCCR1A, WGM11);
    bit_clear(TCCR1B, WGM12);

    // Set the upper counter bound
    OCR1A = countTo;

    clockOption = clockSelect;

}

void timer16_start(void) {

    // Reset the ticks
    TCNT1 = 0;

    // Set the prescaler and start the timer
    switch (clockOption) {
		case DISABLE_TIMER:
			bit_clear(TCCR1B, CS10);
            bit_clear(TCCR1B, CS11);
            bit_clear(TCCR1B, CS12);
		break;
		case CLOCK_SCALE_1:
			bit_set(TCCR1B, CS10);
            bit_clear(TCCR1B, CS11);
            bit_clear(TCCR1B, CS12);
		break;
		case CLOCK_SCALE_8:
			bit_clear(TCCR1B, CS10);
            bit_set(TCCR1B, CS11);
            bit_clear(TCCR1B, CS12);
		break;
		case CLOCK_SCALE_64:
			bit_set(TCCR1B, CS10);
            bit_set(TCCR1B, CS11);
            bit_clear(TCCR1B, CS12);
		break;
		case CLOCK_SCALE_256:
			bit_clear(TCCR1B, CS10);
            bit_clear(TCCR1B, CS11);
            bit_set(TCCR1B, CS12);
		break;
		case CLOCK_SCALE_1024:
			bit_set(TCCR1B, CS10);
            bit_clear(TCCR1B, CS11);
            bit_set(TCCR1B, CS12);
		break;
		case EXT_CLOCK_FALLING:
			bit_clear(TCCR1B, CS10);
            bit_set(TCCR1B, CS11);
            bit_set(TCCR1B, CS12);
		break;
		case EXT_CLOCK_RISING:
			bit_set(TCCR1B, CS10);
            bit_set(TCCR1B, CS11);
            bit_set(TCCR1B, CS12);
		break;
		default:
			bit_clear(TCCR1B, CS10);
            bit_clear(TCCR1B, CS11);
            bit_clear(TCCR1B, CS12);
			return;
		break;
	}

    
    
}

void timer16_stop(void) {
	bit_clear(TCCR1B, CS10);
    bit_clear(TCCR1B, CS11);
    bit_clear(TCCR1B, CS12);
}

BYTE timer16_is_tripped(void) {
    return bit_is_set(TIFR1, OCF1A);
}

BYTE timer16_is_running(void) {
    return TCCR1B & (_BV(CS10) | _BV(CS11) | _BV(CS12));
}

void timer16_clear_flag(void) {
    bit_set(TIFR1, OCF1A);
}

DWORD timer16_get_count(void) {
	return TCNT1;
}

void timer16_enable_int(void) {
    bit_set(TIMSK1, OCIE1A);
}

void timer16_disable_int(void) {
    bit_clear(TIMSK1, OCIE1A);
}
