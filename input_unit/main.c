/*
 * AVR-FFT-DISPLAY:
 *   The control portion.
 *
 * by William Moy, et al.
 *
 * Summary:
 * This program will control the ATtiny84 to take in audio input, analyze
 * it, and send the analysis to a display chip via serial connection.
 *
 * Inputs:
 * 2 ADC: capture audio input from 3.5mm jack as well as a microphone
 * 2 buttons: switch audio inputs, and reset/debugging
 * 
 * Outputs:
 * 1 TWI: transmit data to display chip
 * 2 LEDs: to show which audio input is actively receiving
 *
 *
 * Version History:
 *    v0.0.1    pseudocode and comments
 *    v0.1      system initialization, LED toggle demo 
 *
 * Legal Stuff:
 *    If you use this code, at least credit me :)
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "common.h"
#include "portmap_attiny84.h"
#include "state.h"
#include "timer16.h"
#include "timer8.h"
#include "types.h"

/* Constants */
#define DEBOUNCE_CYCLE_COUNT 196 // ~10ms, assumes 20MHz clock/1024

/* Global variables */
volatile BYTE state;

int main(void) {

    /* Variables */

    /* Initialize global variables */
    state = 0x00;

    /* Set up LEDs as outputs */
    bit_set(DDRA, LED1_BIT);
    bit_set(DDRA, LED2_BIT);

    /* Blink LEDs to show that we're on */
    bit_set(PORTA, LED1_BIT);
    bit_set(PORTA, LED2_BIT);
    _delay_ms(500);
    bit_clear(PORTA, LED1_BIT);
    bit_clear(PORTA, LED2_BIT);
    _delay_ms(500);
    // turn on LED associated with ADC1
    bit_set(PORTA, LED1_BIT); 

    /* Set up push button */
    bit_clear(DDRB, PB_BIT); // set as input
    bit_clear(PORTB, PB_BIT); // <-unnecessary?
    bit_set(MCUCR, ISC00); // set to trigger on rising edge
    bit_set(MCUCR, ISC01);
    bit_set(GIMSK, INT0);
    sei();

    /* Set up TWI */
    // TODO

    while(1);

// system initialization, including:
// -ports:
//   -set up TWI
// -set up ADC capture timer (40kHz? 16-bit timer?)
// -defaults and variables

// start control loop
// read sample from ADC, depending on which input is selected
// fill dat buffer
// compute FFT
// post-process output from code, i.e. frequencies
// send result through TWI
// end loop

    return 0;
}

/* Button IRQ handler */
ISR(INT0_vect) {
    if (isTimer16Running()) {
        // do nothing
    }
    else {
        enableTimer16Interrupt();
        startTimer16(DEBOUNCE_CYCLE_COUNT, CLOCK_SCALE_1024);
    }
}

/* Timer IRQ handler */
ISR(TIM1_COMPA_vect) {
    // if the button is still held after the time interval
    if (bit_is_set(PINB, PB_BIT)) {
        // TODO: finish switching stuff
        // if reading from ADC 2, switch to ADC1
        if (bit_is_set(state, PB_STATE)) {
            bit_clear(state, PB_STATE);
            bit_clear(PORTA, LED2_BIT);
            bit_set(PORTA, LED1_BIT);
        }
        // else, switch to ADC 2
        else {
            bit_set(state, PB_STATE);
            bit_clear(PORTA, LED1_BIT);
            bit_set(PORTA, LED2_BIT);
        }
    }
    disableTimerInterrupt();
    stopTimer();
    clearTimerTripped();
}

// other functions, may be here or eventually in other files:
// button stuff:
//   -ISR that starts the debounce delay timer
//   -sets a flag until the timer's tripped so interrupts can still
//    happen.  we don't want to block sampling.
//   -if we enter the ISR with the flag set, we don't do anything
// timer0 stuff:
//   -ISR checks the button status
//   -if the button's on, then switch inputs and toggle LEDs
//   -clears the button flag to allow presses again
// timer1 stuff?:
//   -ISR fills the buffer with one value, increments offset
//   -when the buffer's full, disable the timer, reset offset
// reset stuff?
// butt stuff
// helper functions (probably going to be in common.c)



/* Possible extra features */
// sleep, maybe with another button, or repurpose reset button
// ???

/* Design questions */
// capture samples while performing FFT?  what are the implications for
// sampling then processing versus processing while sampling?
// pros: 
// -using the cycles in between grabbing samples, therefore more FFT
//  throughput
// cons:
// -added complexity (duh)
// -possibly doubling the memory usage (TODO: look at FFT implementation
//  to see if pre-filling the SAME buffer is possible, but probably not)
//
// instead of using FFT, maybe use Goertzel algorithm?  we're only
// looking at 8 frequencies, so a full FFT is likely unnecessary
// con: we'd have to rename the project!  jay kay ell oh ell
