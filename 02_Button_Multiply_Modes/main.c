/*
 * Two modes button with reset status
 * Created: 26.06.2015 1:34:02
 * Author: Aleksey M.
 * MCU Atmel ATtiny13
 */

#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char counter = 0; // 1 byte of memory
unsigned char latch = 0;

void setup()
{
	// Button on PB1. LEDs on PB2, PB3
	DDRB = 0b00011101;
	PORTB = 0b00100010;
}

int main(void)
{
	setup();
	while (1)
	{
		// Check if the button pressed
		if (!(PINB & (1 << PB1)))
		{
			// Debounce
			_delay_ms(20);

			// Check again
			if (!(PINB & (1 << PB1)) && (latch == 0))
			{
				// Set switch latch to avoid re-entry into the function until the button is pressed
				// Microcontroller is very fast!
				latch = 1;
				counter++;

				// When limit of modes is reached, reset the counter. Turn LEDs "off"
				if (counter > 2)
				{
					counter = 0;
					PORTB &= ~((1 << PB2) | (1 << PB3));

					// Same as:
					// PORTB &= ~(1<<PB2);
					// PORTB &= ~(1<<PB3);
				}
			}
		}

		// When button is unpressed, turn back the latch
		if ((PINB & (1 << PB1)) && (latch == 1))
		{
			latch = 0;
		}

		// Mode 1
		if (counter == 1)
		{
			PORTB |= (1 << PB2);
		}

		// Mode 2
		if (counter == 2)
		{
			PORTB |= (1 << PB3);
		}
	}
}
