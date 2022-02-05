/*
 * Timer Interrupts
 * Created: 24.05.2015 16:55:36
 * Author: Aleksey M.
 * MCU ATtiny13
 */

#define F_CPU 1200000UL
#include <avr/io.h>
#include <avr/interrupt.h>

// #define LED0_ON PORTB = PORTB | (1 << PB0)
// #define LED0_OFF PORTB = PORTB & ~(1 << PB0)

// #define LED2_ON PORTB = PORTB | (1 << PB2)
// #define LED2_OFF PORTB = PORTB & ~(1 << PB2)

#define LED2_TOG PORTB ^= (1 << PB2)
#define LED0_TOG PORTB ^= (1 << PB0)

// Timer/Counter Overflow
ISR(TIM0_OVF_vect)
{
	LED2_TOG;
}

// Timer/Counter Compare Match A - OCR0A
ISR(TIM0_COMPA_vect)
{
	asm("nop"); // Do nothing
}

// Timer/Counter Compare Match B - OCR0B
ISR(TIM0_COMPB_vect)
{
	asm("nop");
}

void setup()
{
	// LEDs on the PB0, PB2
	DDRB = 0b00000101;
	PORTB = 0b00111010;

	// ----------- Timer Setup  ----------- //
	// Start timer T0 with No prescaling
	TCCR0B |= (1 << CS00);

	// Timer/Counter Interrupt Mask Register
	// Overflow interrupt enable
	TIMSK0 |= (1 << TOIE0);

	// Compare Match A, B Interrupt
	// Uncomment to enable
	// TIMSK0 |= (0<<OCIE0A) | (0<<OCIE0B);

	// The Timer (TCNT0) and Output Compare Registers (OCR0A and OCR0B)
	// When value in the TCNT0 == OCR0A CPU can generate compare match interrupt
	// OCR0A = 100;

	// OCR0B compare register value
	// OCR0B = 200;

	// Analog comparator OFF
	ACSR |= (1 << ACD);

	// Reset timer T0 flags
	TIFR0 = 0;
}

int main(void)
{
	// Clears the global interrupt flag in SREG to prevent any form of interrupt occurring
	cli();
	setup();

	// Enable global interrupts
	sei();
	while (1)
	{
		asm("nop");
	}
}

/*
if 
asm("nop") isn't recognized by compiler

Edit make file to: 
option -std=c99 to -std=gnu99

For c99 mode, the compiler does not recognise 'asm', '__asm' ist required instead!
For gnu99 mode, both variants are accepted, 'asm' and '__asm' as well!
*/
