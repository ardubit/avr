/*
 * Timer PWM. Hardware PWM (Pulse-Width Modulation)
 * LED will dim according to Output Compare Register (OCR0A) value
 * Created: 30.05.2015 14:48:31
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested
 */

/*
Output Compare Unit
------------------------
The 8-bit comparator continuously compares TCNT0 with the Output Compare Registers
(OCR0A and OCR0B). The Waveform Generator uses the match signal to generate an output
according to operating mode.

Remember about:
# Timer's Counter value (TCNT0) with
# Output Compare Register value (OCR0A)

********** TIMER MODES: **********

1. Normal Mode
-------------------
Timer count up to [0 - 255]
- A new counter value can be written anytime
- The Output Compare Unit can be used to generate interrupts at some given time/value
Advantages:
- Time ticker
Disadvantages:
- Waveforms generation in Normal mode is not recommended, due it will occupy too much of the CPU time

2. Clear Timer on Compare Match (CTC) Mode
-------------------
Timer count up to [0 - Compare Register] (OCR0A)
- OCR0A defines the top value
- In CTC mode the counter is cleared to zero when the counter value (TCNT0) matches the OCR0A
Advantages:
- Periods counting
- Interrupt generation via defined time

3. Fast PWM Mode:
-------------------
Timer count up to [0 - 255]
	- In non-inverting Compare Output mode
		if TCNT0 == OCR0A, then OC0A will toggle to "0"
		when TCHT0 reach 255, then OC0A will toggle back to "1"
Advantages:
- High frequency of PWM: rectification, and DAC applications

4. Phase Correct PWM Mode:
-------------------
Timer count up to [0 - 255, and then down 255 - 0]
PWM freq. is 1/2 * Fast PWM
Advantages:
- High resolution phase correct PWM waveform
- Centers of pulses are always matched
- Preferred for motor control applications

TODO
-------------------
- Add PWM output voltage calc
*/

#define F_CPU 1200000UL
#include <avr/io.h>
#include <avr/interrupt.h>

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

	// Start timer T0 with prescaler 64.
	TCCR0B |= (1 << CS01) | (1 << CS00);

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

int main(void)
{
	setup();
	
	// Initial value
	// if TCNT0 == OCR0A, then OC0A will toggle to 0
	OCR0A = 250;

	while (1)
	{
		// Application code
	}
	return 0;
}
