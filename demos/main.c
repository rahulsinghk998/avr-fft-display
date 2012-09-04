/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB    |= 1;           /* make the LED pin an output */
    DDRB    &= 0xFD;        /* make the switch pin an input */
    PORTB   &= 0xFD;        /* make it tri-stated */
    
    /* Set up ADC */
    ADCSRA |= ((1 << ADPS2) | (1 << ADPS1)); // prescaler divides Fcpu by 64
    // no need to change ADMUX, keep reference voltage to Vcc and ADC0 selected
    // ADMUX |= (1 << ADLAR); // uncomment to make 10-bit value left-aligned
    ADCSRA |= (1 << ADEN); // enable ADC
    
    
//    while (1) {
//        _delay_ms(10);  /* max is 262.14 ms / F_CPU in MHz */
//        PORTB ^= 1;
//    }

    
    while (1) {
        char i;
        
        if ((PINB & 0x02) > 0) { /* if the button is pressed */
            
            PORTB ^= 1;
            PORTB ^= 1;
            
            
            ADCSRA |= (1 << ADSC); // start conversion
            
            while ( !(ADCSRA & (1 << ADIF)) ) // wait while conversion in progress
                ;
            
            ADCSRA |= (1 << ADIF); // clear ADIF
            
            for (i=0; i<8; i++) {
                if (ADCL & (1 << i))
                    PORTB |= 0x01;
                else
                    PORTB &= 0xFE;
            }
            for (i=0; i<2; i++) {
                if (ADCH & (1 << i))
                    PORTB |= 0x01;
                else
                    PORTB &= 0xFE;            
            }
            
        }
                
    }
    
    return 0;               /* never reached */
}