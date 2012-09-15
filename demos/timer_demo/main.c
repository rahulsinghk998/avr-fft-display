/* Name: main.c
 * Author: William Moy
 * Timer demo for ATtiny84
 * pressing the button will cause the LED to turn on 
 * 
 * HW configuration steps:
 * PB0 is LED output
 * PB1 is button input
 */

#include <avr/io.h>

int main(void)
{
    DDRB    |= 1;           /* make the LED pin an output */
    DDRB    &= 0xFD;        /* make the switch pin an input */
    PORTB   &= 0xFD;        /* make it tri-stated */

    
    while (1) {
        char i;
        
        if ((PINB & 0x02) > 0) { /* if the button is pressed */
            
            // start timer
            
            
            
        }
                
    }
    
    return 0;               /* never reached */
}