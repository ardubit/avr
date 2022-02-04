/*
 * Simple button. Press the button for LED blinking
 * Created: 23.05.2015 23:14:09
 * Author: Aleksey M.
 * MCU Atmel ATtiny13
 */

#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>

// Here is another way that make code more readble and simple
// The compiler will replace LED_ON with the value of PORTB |= (1<<PB0) when compilation is started
#define LED_ON PORTB |= (1 << PB2)	 // Set HIGH
#define LED_OFF PORTB &= ~(1 << PB2) // Set LOW

// Simple configuration function.
void setup()
{
	DDRB = 0b00011101;
	PORTB = 0b00100010;
}

int main(void)
{
	// Run configuration function
	setup();

	while (1)
	{
		
		/* 
		Button is on PB1 with internal pullup resistor to VCC, 
		when button is pressed a logic level will change from 1 to 0. 
		Conditions in parentheses of the function require a logic "true" for executing code in the "{}".
		The (PINB & (1<<PB1) gives "1" and it's a logic true, but button is not pressed 
		and that's why we use "!" to reverse it.
		*/

		if (!(PINB & (1 << PB1)))
		{
			// This is the place where #define works
			LED_ON;
			_delay_ms(250);
			LED_OFF;
			_delay_ms(250);
		}
	}
}
