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
 *    elm-chan's code is used to process FFTs, thanks be to elm-chan.
 */

/* PSEUDOCODE HERE */

// system initialization, including:
// -ports:
//   -IO stuff
//   -should probably do a quick test of IO...LED flash would be nice
//   -set up TWI
// -set up button interrupts
// -set up debouncing timer
// -defaults and variables

// start control loop
// read sample from ADC, depending on which input is selected
// fill dat buffer
// compute FFT
// post-process output from code, i.e. frequencies
// send result through TWI
// end loop


// other functions, may be here or eventually in other files:
// timer stuff
// button stuff:
//   -input select, LED change and whatnot
// reset stuff?
// butt stuff
// helper functions (probably going to be in common.c)


/* Possible extra features */
// sleep, maybe with another button, or repurpose reset button
// ???
