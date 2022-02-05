/*
 * Blinking LED - 1 Hz
 * Created: 23.05.2015 21:19:05
 * Author: Aleksey M.
 * MCU Atmel ATtiny13
 */

// MCU Frequency - 1,2 MHz
// 9,6 MHz default clock with prescaling of 8. See page 26
#define F_CPU 1200000UL
#define LED PB3

// Input/Output definition library. Needs for work with registers names
#include <avr/io.h>

// The delay library requires to define F_CPU, it was done before
#include <util/delay.h>

int main(void)
{
	/* 
	Define direction for port pins 
	---------------------------------------
	"0" - input, "1" - output
	Unused "*" pins config to - Inputs with enabled internal pull-up resistors

	PB0 - Button 
	PB1 - *
	PB2	- *
	PB3 - LED - 2,2k - GND
	PB4 - *
	PB5 - Reset (input)

	PB6, PB7 are Read-only bits

	DC Current per I/O Pin 40.0mA
	LEDred Vf = 1,65V, I = 1,5mA
	VCC = 5V
	R = (5V - 1,65V) / 2 = 2,23K = 2,2K
	*/

	DDRB = 0b00001000;

	/* 
	Define pullups resistors to "VCC" for inputs 
	or logic states for outputs
	Input "1" - pullup enable, "0" - pullup disable
	Output "1" - logic one, "0" - logic zero
	*/

	PORTB = 0b00110111;

	/* 
	The program executes without button. Settings for button was done 
	for future playgrounds
	*/

	while (1)
	{
		// Toggle bit. LED ON/OFF
		PORTB ^= (1 << LED);

		// f = 1/T, T = 1 sec
		// There are two states in the period: led on, led off

		// Delay time in milliseconds
		_delay_ms(100);

		// Classic "Hello World" program
		//PORTB |= (1<<PB2);
		//_delay_ms (500);
		//PORTB &= ~(1<<PB2);
		//_delay_ms (500);
	}
}
