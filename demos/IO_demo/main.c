/* Name: main.c
 * Author: William Moy
 * EDit: Edward Gabriel
 * I/O demo for ATtiny2313
 * holding the button will allow the LED to flash
 * 
 * HW configuration steps:
 * PA0 is LED output
 * PA1 is button input
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRA    |= 1;           /* make the LED pin an output */
    DDRA    &= 0xFD;        /* make the switch pin an input */
    PORTA   &= 0xFD;        /* make it tri-stated */

    
    while (1) {
        char i;
        if ((PINA & 0x02) > 0) { /* if the button is pressed */
            _delay_ms(10);
            PORTA ^= 1;
        }
    }
    
    return 0;               /* never reached */
}