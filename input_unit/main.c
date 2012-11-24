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
 *    v0.2      completed implementation, untested on hardware
 *
 * Legal Stuff:
 *    If you use this code, at least credit me :)
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include "adc.h"
#include "common.h"
#include "goertzel.h"
#include "portmap_attiny84.h"
#include "state.h"
#include "suart.h"
#include "timer16.h"
#include "timer8.h"
#include "types.h"

/* Constants */
#define DEBOUNCE_CYCLE_COUNT 10000 // ~10ms, assumes 8MHz F_cpu/8
#define SAMPLE_CYCLE_COUNT   200   // 40kHz

/* Global variables */
volatile BYTE state;
volatile BYTE output[8];

int main(void) {
    /* Initialize global variables */
    state = 0x00;

    /* Set up LEDs as outputs */
    bit_set(DDRB, LED1_BIT);
    bit_set(DDRB, LED2_BIT);

    /* Blink LEDs to show that we're on */
    bit_set(PORTB, LED1_BIT);
    bit_set(PORTB, LED2_BIT);
    delay_ms(250);
    bit_clear(PORTB, LED1_BIT);
    bit_clear(PORTB, LED2_BIT);
    delay_ms(250);
    // turn on LED associated with ADC1
    bit_set(PORTB, LED1_BIT); 

    /* Set up push button */
    bit_clear(DDRB, PB_BIT); // set as input
    bit_clear(PORTB, PB_BIT); // <-unnecessary?
    bit_set(MCUCR, ISC00); // set to trigger on rising edge
    bit_set(MCUCR, ISC01); // set to trigger on rising edge
    bit_set(GIMSK, INT0); // enable external pushbutton interrupt
    sei(); // enable global interrupts
    timer16_enable_int(); // enable timer interrupts
    timer16_init(DEBOUNCE_CYCLE_COUNT, CLOCK_SCALE_8); // initialize timer

    /* Set up SUART */
    suart_init();

    /* Set up ADC */
    adc_init(); // initialize ADC hardware
    adc_set_prescaler(ADC_CLK_DIV_8); // run ADC clock at 1MHz
    adc_select(ADC0_ADC3_1); // read from ADC0, compare with ADC3, gain x1 for testing purposes
    adc_enable_int(); // enable ADC interrupt on completed conversion

    /* Set up ADC capture timer */
    timer8_enable_int(); // enable sampling timer, 32kHz
    timer8_init(SAMPLE_CYCLE_COUNT, CLOCK_SCALE_1); // initialize timer

    while(1);

    return 0;
}

/* Button IRQ handler */
ISR(INT0_vect) {
    if (timer16_is_running()) {
        // do nothing
    }
    else {
        timer16_start(); // start 10ms debouncing timer
    }
}

/* Timer IRQ handler */
ISR(TIM1_COMPA_vect) {
    // if the button is still held after the time interval
    if (bit_is_set(PINB, PB_BIT)) {
        while (adc_is_running()) // don't interrupt sampling
            ;
        // if reading from ADC 2, switch to ADC1
        if (bit_is_set(state, PB_STATE)) {
            adc_select(ADC0_ADC3_1);
            bit_clear(state, PB_STATE);
            bit_clear(PORTB, LED2_BIT);
            bit_set(PORTB, LED1_BIT);
        }
        // else, switch to ADC 2
        else {
            adc_select(ADC1_ADC3_1);
            bit_set(state, PB_STATE);
            bit_clear(PORTB, LED1_BIT);
            bit_set(PORTB, LED2_BIT);
        }
    }
    timer16_stop();
    timer16_clear_flag();
}

/* Start A to D conversion of audio sample */
ISR(TIM0_COMPA_vect) {
    adc_start();
}

/* When a sample is taken, do calculations, and get frequency if ready */
ISR(ADC_vect) {
    goertzel_process_sample(adc_get_value8());
    if (goertzel_is_ready()) {
        BYTE i;
        DWORD output16[8];
        timer8_stop(); // stop capturing samples
        goertzel_process_magnitudes(output16);
        for (i=0; i<8; i++)
            output[i] = sq16_to_bar8(output16[i]); // convert to displayable
        for (i=0; i<8; i++)
            suart_xmit(output[i]); // send frame
        timer8_start(); // start capturing samples again
    }
}