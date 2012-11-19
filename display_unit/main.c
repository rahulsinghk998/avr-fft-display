/* Name: main.c
 * Author: Brian Dunlay
 * Original Code from: http://bildr.org/2011/02/74hc595/
 * LED Matrix Controller for the ATtiny2313
 * 
 * HW configuration steps:
 * PAX :: (SER) 	SR Serial Data Input
 * PAX :: (SRCLK) 	SR Serial Clock
 * PAX :: (RCLK)	SR Register Clock
 * PAX :: (SRCLR)	SR Serial Clear
 * PAX :: (OE)		SR Output Enable
 */
 
#include <avr/io.h>
#include <util/delay.h>
#include "shift.h"

int main()
{
	DDRB = 0xFF;
	PORTB = 0x00;

	while(1)
	{
		lineRefresh();
	}
	return 0;
}

