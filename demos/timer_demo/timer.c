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

void setup_timer(int countTo, int clockSelect) {

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

    OCR0B = countTo;
    
}

void stop_timer(void) {
    TCCR0B &= 0xF8;
}


BYTE get_timer_value(void) {
	return TCNT0;
}
