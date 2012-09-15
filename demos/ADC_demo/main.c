/* Name: main.c
 * Author: William Moy
 * ADC demo for ATtiny84
 * holding the button will cause the ADC to read a sample and write the binary values to the LED
 * 
 * HW configuration steps:
 * PB0 is LED output
 * PB1 is button input
 * PA0 is ADC input
 */

#include <avr/io.h>

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
    
    
    while (1) {
        char i;
        
        if ((PINB & 0x02) > 0) { /* if the button is pressed */
            
            
            ADCSRA |= (1 << ADSC); // start conversion
            
            while ( !(ADCSRA & (1 << ADIF)) ) // wait while conversion in progress
                ;
            
            ADCSRA |= (1 << ADIF); // clear ADIF
            
            /* output the sample bits to the LED */
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