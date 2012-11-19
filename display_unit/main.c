/* Name: main.c
 * Author: Brian Dunlay
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

#define number_of_74hc595s 1 //How many of the shift registers are there daisey chained?

void shift(int, int, int, unsigned long);

int main(){
	DDRB = 0xFF;
	PORTB = 0x00;
	char counter = 0;

	while(1){
		counter++; 				// Counter used for displaying a number in binary via the shift register
		shift(PB1, PB2, PB3, counter);		// PB1 = SERCLK  PB2 = RCLK  PB3 = SER
		_delay_ms(500);
		shift(PB1, PB2, PB3, 0x00);		// Set all pins to off
		_delay_ms(500);
	}
	return 0;
}

void shift(int SRCLK_Pin, int RCLK_Pin, int SER_Pin, unsigned long data){
	PORTB &= ~(1 << RCLK_Pin); 				// Set the register-clock pin low

	int i;
	for (i = 0; i < (8 * number_of_74hc595s); i++){	// Now we are entering the loop to shift out 8+ bits

		PORTB &= ~(1 << SRCLK_Pin); 			// Set the serial-clock pin low

		PORTB |= (((data&(0x01<<i))>>i) << SER_Pin ); 	// Go through each bit of data and output it

		PORTB |= (1 << SRCLK_Pin); 			// Set the serial-clock pin high

		PORTB &= ~(((data&(0x01<<i))>>i) << SER_Pin );	// Set the datapin low again	
	}

	PORTB |= (1 << RCLK_Pin);				// Set the register-clock pin high to update the output of the shift-register
}

