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
#include "types.h"
#include "timer.h"

int main(void)
{
    DDRB    |= 1;           /* make the LED pin an output */
    DDRB    &= 0xFD;        /* make the switch pin an input */
    PORTB   &= 0xFD;        /* make it tri-stated */

    /* set up the timer */

    BYTE waiting = 0;
    while (1) {
        BYTE i;
        
        if (!waiting && (PINB & 0x02) > 0) { /* if the button is pressed */
            
            // start timer
            
            
            
        }
                
    }
    
    return 0;               /* never reached */
}
