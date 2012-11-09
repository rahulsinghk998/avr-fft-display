/* Name: main.c
 * Author: William Moy
 * Timer demo for ATtiny84
 * pressing the button will cause the LED to turn on 
 * 
 * HW configuration steps:
 * PB0 is LED output
 * PB1 is button input
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include "common.h"
#include "timer.h"
#include "types.h"

int main(void)
{
    DDRB    |= 1;           /* make the LED pin an output */
    DDRB    &= 0xFD;        /* make the switch pin an input */
    PORTB   &= 0xFD;        /* make it tri-stated */

    /* set up the timer */
    BYTE timerHits = 0;
    while (1) {
        
        if ((PINB & 0x02) > 0) { /* if the button is pressed */
            /* if the timer is running, keep going */
            if (isTimerRunning()) {
                continue;
            }
            /* if the timer isn't running, start it */
            else {
                enableTimerInterrupt();
                startTimer(255, CLOCK_SCALE_1024);
                sei();
                timerHits = 0;
            }
        }
        /* if the button is not pressed, stop and clear timer */
        else
        {
            cli();
            stopTimer();
            disableTimerInterrupt();
            timerHits = 0;
            clearTimerTripped();
        }
               
    }
    
    return 0;               /* never reached */
}

ISR(TIM0_COMPA_vect) {
    if (++timerHits >= 4) {
        PORTB ^= 1; // toggle LED when limit reached
        timerHits = 0;
    }
}
