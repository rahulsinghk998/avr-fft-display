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
#include "common.h"
#include "types.h"
#include "timer.h"

int main(void)
{
    DDRB    |= 1;           /* make the LED pin an output */
    DDRB    &= 0xFD;        /* make the switch pin an input */
    PORTB   &= 0xFD;        /* make it tri-stated */

    /* set up the timer */
    BYTE timerHits = 0;
    while (1) {
        
        if ((PINB & 0x02) > 0) { /* if the button is pressed */
            
            if (isTimerRunning()) {
                if (!isTimerTripped()) {
                    continue;
                }
                else
                {
                    clearTimerTripped();
                    if (++timerHits >= 4) {
                        PORTB ^= 1;
                        timerHits = 0;
                    }
                }
            }
            else {
                startTimer(255, CLOCK_SCALE_1024);
                timerHits = 0;
            }
        }
        else
        {
            stopTimer();
            timerHits = 0;
            clearTimerTripped();
        }
               
    }
    
    return 0;               /* never reached */
}
