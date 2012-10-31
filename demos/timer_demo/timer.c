/*
* timer.c
* William Moy
* Timer interface for 8-bit ATtiny AVR
* Designed on ATtiny84
*
*/

#include "timer.h"
#include "types.h"
#include <avr/sfr_defs.h>
#include <avr/common.h>
#include <avr/io.h>

int timerHits = 0;

void startTimer(int countTo, int clockSelect) {

    // Set Clear Timer on Compare (CTC) Mode
    TCCR0A &= ~_BV(WGM00);
    TCCR0A |= _BV(WGM01);
    TCCR0B &= ~_BV(WGM02);

    // Set the upper counter bound
    OCR0A = countTo;
	
    // Reset the ticks
    TCNT0 = 0;

    // Set the prescaler and start the timer
    switch (clockSelect) {
		case DISABLE_TIMER:
			TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00));
		break;
		case CLOCK_SCALE_1:
			TCCR0B |= (_BV(CS00));
			TCCR0B &= ~(_BV(CS02)| _BV(CS01));
		break;
		case CLOCK_SCALE_8:
			TCCR0B |= (_BV(CS01));
			TCCR0B &= ~(_BV(CS02) | _BV(CS00));
		break;
		case CLOCK_SCALE_64:
			TCCR0B |= _BV(CS01) | _BV(CS00);
			TCCR0B &= ~(_BV(CS02));
		break;
		case CLOCK_SCALE_256:
			TCCR0B |= _BV(CS02);
			TCCR0B &= ~(_BV(CS01) | _BV(CS00));
		break;
		case CLOCK_SCALE_1024:
			TCCR0B |= (_BV(CS02) | _BV(CS00));
			TCCR0B &= ~(_BV(CS01));
		break;
		case EXT_CLOCK_FALLING:
			TCCR0B |= (_BV(CS02) | _BV(CS01));
			TCCR0B &= ~(_BV(CS00));
		break;
		case EXT_CLOCK_RISING:
			TCCR0B |= (_BV(CS02) | _BV(CS01) | _BV(CS00));
		break;
		default:
			TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00));
			return;
		break;
	}

    
    
}

void stopTimer(void) {
    TCCR0B &= 0xF8;
}

BYTE isTimerTripped(void) {
    return bit_is_set(TIFR0, OCF0A);
}

void clearTimerTripped(void) {
    TIFR0 = _BV(OCF0A);
}

BYTE getTimerValue(void) {
	return TCNT0;
}
