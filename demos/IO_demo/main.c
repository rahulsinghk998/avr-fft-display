/* Name: main.c
 * Author: William Moy
 * I/O demo for ATtiny84
 * holding the button will allow the LED to flash
 * 
 * HW configuration steps:
 * PB0 is LED output
 * PB1 is button input
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB    |= 1;           /* make the LED pin an output */
    DDRB    &= 0xFD;        /* make the switch pin an input */
    PORTB   &= 0xFD;        /* make it tri-stated */

    
    while (1) {
        char i;
        if ((PINB & 0x02) > 0) { /* if the button is pressed */
            _delay_ms(10);
            PORTB ^= 1;
        }
    }
    
    return 0;               /* never reached */
}