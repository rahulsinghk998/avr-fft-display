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
    TCCR0A &= ~(_BV(WGM00) | _BV(WGM02));
    TCCR0A |= _BV(WGM01);

    // Set the upper counter bound
    OCR0A = countTo;

    clockOption = clockSelect;

}

void timer8_start(void) {

    // Reset the ticks
    TCNT0 = 0;

    // Set the prescaler and start the timer
    TCCR0B &= 0b11111000; // zero out the first 3 bits
    TCCR0B |= clockOption;

}

void timer8_stop(void) {
    TCCR0B &= ~(_BV(CS00) | _BV(CS01) | _BV(CS02));
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
