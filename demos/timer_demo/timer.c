/*
* timer.c
* William Moy
* Timer interface for 
*
*/

#include "timer.h"

int timerHits = 0;

void setup_timer(int countTo, int clockSelect) {

	switch (clockSelect) {
		case DISABLE_TIMER:
			TCCR0B &= ~(CS02 << 1 | CS01 << 1 | CS00 << 1);
			return;
		break;
		case CLOCK_SCALE_1:
			TCCR0B |= (CS00 << 1);
			TCCR0B &= ~(CS02 << 1 | CS01 << 1);
		break;
		case CLOCK_SCALE_8:
			TCCR0B |= (CS01 << 1);
			TCCR0B &= ~(CS02 << 1 | CS00 << 1);
		break;
		case CLOCK_SCALE_64:
			TCCR0B |= (CS01 << 1 | CS00 << 1);
			TCCR0B &= ~(CS02 << 1);
		break;
		case CLOCK_SCALE_256:
			TCCR0B |= (CS02 << 1);
			TCCR0B &= ~(CS01 << 1 | CS00 << 1);
		break;
		case CLOCK_SCALE_1024:
			TCCR0B |= (CS02 << 1 | CS00 << 1);
			TCCR0B &= ~(CS01 << 1);
		break;
		case EXT_CLOCK_FALLING:
			TCCR0B |= (CS02 << 1 | CS01 << 1);
			TCCR0B &= ~(CS00 << 1);
		break;
		case EXT_CLOCK_RISING:
			TCCR0B |= (CS02 << 1 | CS01 << 1 | CS00 << 1);
		break;
		default:
			TCCR0B &= ~(CS02 << 1 | CS01 << 1 | CS00 << 1);
			return;
		break;
	}


    
}

void start_timer(int num) {

}

void stop_timer(int num) {

}


BYTE get_timer_value(void) {
	return TCNT0;
}