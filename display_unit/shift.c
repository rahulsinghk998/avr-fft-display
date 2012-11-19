/* Name: main.c
 * Author: Brian Dunlay
 * Common functions for LED Matrix for the ATtiny2313
 */

#ifndef SHIFT_H
#define SHIFT_H
#endif

#include "shift.h"
#include <avr/io.h>

void lineRefresh(int column)
{
 				// Counter used for displaying a number in binary via the shift register
		shift(PB1, PB2, PB3, 0x1<<column);		// PB1 = SERCLK  PB2 = RCLK  PB3 = SER
//		_delay_ms(10);
//		shift(PB1, PB2, PB3, 0x00);		// Set all pins to off
//		_delay_ms(10);
	}
}

void shift(int SRCLK_Pin, int RCLK_Pin, int SER_Pin, unsigned long data)
{
	PORTB &= ~(1 << RCLK_Pin); 				// Set the register-clock pin low

	int i;
	for (i = 0; i < (8); i++)
	{	// Now we are entering the loop to shift out 8+ bits

		PORTB &= ~(1 << SRCLK_Pin); 			// Set the serial-clock pin low
		//_delay_ms(100);
		PORTB |= (((data&(0x01<<i))>>i) << SER_Pin ); 	// Go through each bit of data and output it
		//_delay_ms(100);
		PORTB |= (1 << SRCLK_Pin); 			// Set the serial-clock pin high
		//_delay_ms(100);
		PORTB &= ~(((data&(0x01<<i))>>i) << SER_Pin );	// Set the datapin low again
		//_delay_ms(100);
	}

	PORTB |= (1 << RCLK_Pin);				// Set the register-clock pin high to update the output of the shift-register
}


