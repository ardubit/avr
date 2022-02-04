/*
 * Blinking LED - 1 Hz
 * Created: 23.05.2015 21:19:05
 * Author: Aleksey M.
 * MCU Atmel ATtiny13
 */

// MCU Frequency - 1,2 MHz
// 9,6 MHz default clock with prescaling of 8. See page 26
#define F_CPU 1200000UL

// Input/Output definition library for work with registers names
#include <avr/io.h>

// The library required to define F_CPU, it was done before
#include <util/delay.h>

int main(void)
{

	// Define direction for port pins. "0" - input, "1" - output
	// Button is on PB1. LED attached to the PB2 - 1k - GND, others are outputs, except PB5
	// PB5 is RESET
	DDRB = 0b00011101;

	// Define pullups resistors to "VCC" or output logic states of pins
	// Input "1" - pullup enable, "0" - pullup disable
	// Output "1" - logic one, "0" - logic zero
	// PB1, PB5 pullup enable
	PORTB = 0b00100010;

	// The program executed without button. 
	// Settings for button was done for future playgrounds
	while (1)
	{
		// Toggle bit! LED ON/OFF
		PORTB ^= (1 << PB2);

		// f = 1/T, T = 1 sec, There are two states in the period: led on, led off
		// Set the delay in milliseconds
		_delay_ms(500);

		// Classic "Hello World" program
		//PORTB |= (1<<PB2);
		//_delay_ms (500);
		//PORTB &= ~(1<<PB2);
		//_delay_ms (500);
	}
}
