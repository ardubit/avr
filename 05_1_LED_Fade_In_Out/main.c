/*
 * LED Fade In, Fade Out. (Heartbeat) Hardware PWM
 * Created: 30.05.2015 14:48:31
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested
 */

#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>

#define FADE_DELAY 10
#define CORRECTION 165

void setup()
{
	/*
	PB0 - LED - 2,2k - GND, (Hardware PWM Output)
	PB1, PB2, PB3, PB4 - Unused "*"
	PB5 - Reset (input)
	*/

	// Always set all bits!
	DDRB |= (0 << PB5) | (0 << PB4) | (0 << PB3) | (0 << PB2) | (0 << PB1) | (1 << PB0);
	PORTB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1) | (0 << PB0);

	// Analog comparator OFF
	ACSR |= (1 << ACD);

	/*
	Timer Setup
	---------------------------------------
	*/

	// Start timer T0. No prescaler
	// TCCR0B |= (1 << CS00);

	// Start timer T0. Prescaler 8
	// TCCR0B |= (1 << CS01);

	// Start timer T0. Prescaler 64
	TCCR0B |= (1 << CS01) | (1 << CS00);

	// Start timer T0. Prescaler 256
	// TCCR0B |= (1 << CS02) | (0 << CS01) | (0 << CS00);

	// Start timer T0. Prescaler 1024
	// TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);

	// Enable Fast PWM
	TCCR0A |= (1 << WGM01) | (1 << WGM00);

	/*
	Non-inverting Mode:
	COM0A1, COM0A0 [1,0] Clear OC0A on Compare Match, set OC0A at TOP
	Inverting Mode:
	COM01, COM00 [1,1] Set OC0A on Compare Match, clear OC0A at TOP
	*/

	// Non-inverting Compare Output Mode
	TCCR0A |= (1 << COM0A1) | (0 << COM0A0);

	// Reset timer T0 flags.
	TIFR0 = 0;
}

void fade_out()
{
	// Here could be a trap with <= and int8!
	for (uint8_t i = 0; i < (255 - CORRECTION); i++)
	{
		OCR0A = i;
		_delay_ms(FADE_DELAY);
	}
}

void fade_in()
{
	for (uint8_t i = (255 - CORRECTION); i > 0; i--)
	{
		OCR0A = i;
		_delay_ms(FADE_DELAY);
	}
}

int main(void)
{
	setup();
	while (1)
	{
		fade_out();
		fade_in();
	}
	return 0;
}
