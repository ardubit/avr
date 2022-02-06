/*
 * Simple button. Press the button for blinking an LED
 * Created: 23.05.2015 23:14:09
 * Author: Aleksey M.
 * MCU Atmel ATtiny13
 * Tested
 */

#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>

/*
Here is another way that makes code more readable and simple.
The C Preprocessor will replace LED_ON with the value
of PORTB |= (1<<PB0) when compilation is started
*/

#define LED_ON PORTB |= (1 << PB3)	 // Set HIGH
#define LED_OFF PORTB &= ~(1 << PB3) // Set LOW

// Configuration function
void setup()
{
	/*
	Define direction for port pins
	---------------------------------------
	"0" - input, "1" - output
	Unused "*" pins config to - Inputs with enabled internal pull-up resistors

	PB0 - Button - GND
	PB1 - *
	PB2	- *
	PB3 - LED - 2,2k - GND
	PB4 - *
	PB5 - Reset (input)
	*/

	DDRB = 0b00001000;
	PORTB = 0b00110111;
}

int main(void)
{
	// Run configuration
	setup();
	while (1)
	{

		/*
		Button is on PB0 with internal pullup resistor to VCC,
		when button is pressed a logic level will change from 1 to 0.
		Conditions in parentheses of the function require a logic "true" for executing code in the "{}".
		The (PINB & (1<<PB1) gives "1" and it's a logic true, but button is not pressed
		and that's why we should use "!" to reverse it.
		*/

		if (!(PINB & (1 << PB0)))
		{
			// This is the place where #define works
			LED_ON;
			_delay_ms(250);
			LED_OFF;
			_delay_ms(250);
		}
	}
	return 0;
}
