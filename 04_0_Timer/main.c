/*
 * Timer Interrupts. LED toggle in the Timer's ISR.
 * Created: 24.05.2015 16:55:36
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested
 */

#define F_CPU 1200000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// #define LED1_ON PORTB = PORTB | (1 << PB3)
// #define LED1_OFF PORTB = PORTB & ~(1 << PB3)

// #define LED2_ON PORTB = PORTB | (1 << PB4)
// #define LED2_OFF PORTB = PORTB & ~(1 << PB4)

#define LED1_TOG PORTB ^= (1 << PB3)
// #define LED2_TOG PORTB ^= (1 << PB4)

// Timer/Counter Overflow
ISR(TIM0_OVF_vect)
{
	LED1_TOG;
	_delay_ms(50); // It's only for debug. Do not use delays in the ISRs!
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
	/*
	PB0, PB1, PB2 - *
	PB3 - LED - 2,2k - GND
	PB4 - LED - 2,2k - GND
	PB5 - Reset (input)
	*/

	DDRB = 0b00011000;
	PORTB = 0b00100111;

	/* 
	Timer Setup  
	---------------------------------------
	Timer T0 (8-bit) counts up to (or bi-directional) = (2^8) - 1 = 255, 
	then timer's register TCNT0 will overflow.
	*/

	/*
	Timer/Counter Control Register A
	---------------------------------------
	- Timer mode: Normal port operation
	- Waveform generation: "OFF", OC0A disconnected
	
	Normal Mode set by default:
	COM0A1, COM0A0 – [0,0]
	*/

	// Normal Mode
	TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0));

	// Start timer T0 with No prescaling
	TCCR0B |= (1 << CS00);

	// Timer/Counter Interrupt Mask Register. Overflow interrupt enable
	TIMSK0 |= (1 << TOIE0);

	// Compare Match A, B Interrupt
	// Uncomment to enable
	// TIMSK0 |= (0<<OCIE0A) | (0<<OCIE0B);

	/*
	The Timer (TCNT0) and Output Compare Registers (OCR0A and OCR0B).
	When value in the TCNT0 == OCR0A CPU can generate compare match interrupt
	*/

	// OCR0A,B compare register values
	// OCR0A = 100;
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
	return 0;
}

/*
if
asm("nop") isn't recognized by compiler

Edit make file to:
option -std=c99 to -std=gnu99

For c99 mode, the compiler does not recognise 'asm', '__asm' ist required instead!
For gnu99 mode, both variants are accepted, 'asm' and '__asm' as well!
*/
