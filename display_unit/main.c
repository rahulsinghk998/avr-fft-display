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
#include "common.h"
 
int main()
{
	DDRB = 0xFF;
	PORTB = 0x00;
	char counter = 0;

	while(1)
	{
		counter = 128; 				// Counter used for displaying a number in binary via the shift register
		shift(PB1, PB2, PB3, counter);		// PB1 = SERCLK  PB2 = RCLK  PB3 = SER
		_delay_ms(10);
		shift(PB1, PB2, PB3, 0x00);		// Set all pins to off
		_delay_ms(10);
	}
	return 0;
}

