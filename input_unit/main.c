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
 *    v0.0.1 = pseudocode and comments
 *
 * Legal Stuff:
 *    If you use this code, at least credit me :)
 */

/* PSEUDOCODE HERE */

#include "common.h"
// #ifdef ATTINY84???
#include "portmap_attiny84.h"
// #endif
#include "types.h"

int main(void) {
    return 0;
}

// system initialization, including:
// -ports:
//   -IO stuff
//   -should probably do a quick test of IO...LED flash would be nice
//   -set up TWI
// -set up button interrupts (positive edge triggered)
// -set up debouncing timer (8-bit timer)
// -set up ADC capture timer (40kHz? 16-bit timer?)
// -defaults and variables

// start control loop
// read sample from ADC, depending on which input is selected
// fill dat buffer
// compute FFT
// post-process output from code, i.e. frequencies
// send result through TWI
// end loop


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
