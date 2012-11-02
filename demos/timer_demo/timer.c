/*
* timer.c
* William Moy
* Timer interface for 8-bit ATtiny AVR
* Designed on ATtiny84
*
*/

#include "timer.h"
#include "types.h"
#include "common.h"
#include <avr/io.h>
#include <avr/sfr_defs.h>

int timerHits = 0;

void startTimer(int countTo, int clockSelect) {

    // Set Clear Timer on Compare (CTC) Mode
    bit_clear(TCCR0A, _BV(WGM00));
    bit_set(TCCR0A, _BV(WGM01));
    bit_clear(TCCR0B, _BV(WGM02));

    // Set the upper counter bound
    OCR0A = countTo;

    // Reset the ticks
    TCNT0 = 0;

    // Set the prescaler and start the timer
    switch (clockSelect) {
		case DISABLE_TIMER:
			bit_clear(TCCR0B, _BV(CS00) | _BV(CS01) | _BV(CS02));
		break;
		case CLOCK_SCALE_1:
			bit_set(TCCR0B, _BV(CS00));
			bit_clear(TCCR0B, _BV(CS01) | _BV(CS02));
		break;
		case CLOCK_SCALE_8:
			bit_set(TCCR0B, _BV(CS01));
			bit_clear(TCCR0B, _BV(CS00) | _BV(CS02));
		break;
		case CLOCK_SCALE_64:
			bit_set(TCCR0B, _BV(CS00) | _BV(CS01));
			bit_clear(TCCR0B, _BV(CS02));
		break;
		case CLOCK_SCALE_256:
			bit_set(TCCR0B, _BV(CS02));
			bit_clear(TCCR0B, _BV(CS00) | _BV(CS01));
		break;
		case CLOCK_SCALE_1024:
			bit_set(TCCR0B, _BV(CS00) | _BV(CS02));
			bit_clear(TCCR0B, _BV(CS01));
		break;
		case EXT_CLOCK_FALLING:
			bit_set(TCCR0B, _BV(CS01) | _BV(CS02));
			bit_clear(TCCR0B, _BV(CS00));
		break;
		case EXT_CLOCK_RISING:
			bit_set(TCCR0B, _BV(CS00) | _BV(CS01) | _BV(CS02));
		break;
		default:
			bit_clear(TCCR0B, _BV(CS00) | _BV(CS01) | _BV(CS02));
			return;
		break;
	}

    
    
}

void stopTimer(void) {
    bit_clear(TCCR0B, _BV(CS00) | _BV(CS01) | _BV(CS02));
}

BYTE isTimerTripped(void) {
    return bit_is_set(TIFR0, OCF0A);
}

BYTE isTimerRunning(void) {
    return TCCR0B & (_BV(CS00) | _BV(CS01) | _BV(CS02));
    //return bit_is_set(TCCR0B, CS00 | CS01 | CS02);
}

void clearTimerTripped(void) {
    bit_set(TIFR0, _BV(OCF0A));
}

BYTE getTimerValue(void) {
	return TCNT0;
}
