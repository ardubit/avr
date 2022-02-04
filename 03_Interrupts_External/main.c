/*
 * External INT0 interrupt. (Button interrupt)
 * Created: 24.05.2015 1:28:05
 * Author: Aleksey M.
 * MCU ATtiny13
 */

#define F_CPU 1200000UL
#include <avr/io.h>

#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char int0_flag = 0;

/*
MCU execute two tasks: blinking LED_1 on BP2 and toggle LED_2 on BP3 when interrupt is occur. 
It may seems that everything is happening at once, but it's not true. Everything is going one by one.
Interrupts is a way to implement multitasking. 
*/

// This is an interrupt vector – "External Interrupt Request 0"
// For all vectors look at the table 9-1, page 45 of datasheet
// When the interrupt occur, this code will executed

ISR(INT0_vect)
{
	PORTB ^= (1 << PB3);

	// ---------------------------------------------------------------
	// Simple SOFTWARE DEBOUNCE without _delay_ms() function | Start
	// ---------------------------------------------------------------

	// // Delays in the interrupt vectors is not a good idea!
	// // With delay functions microcontroller doing nothing, another interrupt could occur anytime
	// // thus microcontroller definitely will lose events or data.

	// if (!(PINB & (1<<PB1)) && (int0_flag == 0))
	// {
	// 	int0_flag = 1;
	// 	 // Toggle LED
	// 	PORTB ^= (1<<PB3);
	// }

	// int0_flag = 0;

	// // To avoid a repeat execution – clean up (before exit from vector) in the GIFR register flag!
	// // Set a logical "1" into bit 6, GIFR General Interrupt Flag Register
	// // The flag is cleared when the interrupt routine is executed. Page 48

	// GIFR |= (1<<INTF0);

	// ---------------------------------------------------------------
	// Simple SOFTWARE DEBOUNCE without _delay_ms() function | End
	// ---------------------------------------------------------------
}

void setup()
{
	// For hardware debounce reduction, add in parallel to the button 100n capacitor

	DDRB = 0b00011101;
	PORTB = 0b00100010;

	// General Interrupt Mask Register
	// The bit 6 turn ON Interrupt Request on INT0 pin

	GIMSK = (1 << INT0);

	// MCU Control Register
	// Contains control bits for interrupt sense control
	// The falling edge of INT0 generates an interrupt request
	// All of the settings look at the table 9-2, page 47

	MCUCR |= (1 << ISC01);
	MCUCR &= ~(1 << ISC00);
}

void blink()
{
	PORTB ^= (1 << PB2);
	_delay_ms(500);
}

int main(void)
{
	setup();

	/* 
	Interrupts should globally enabled in the code. By default, interrupts are globally switched OFF.
	The global interrupt flag is maintained in the "I" bit of the status register (SREG). Page 9.
	The cli(); disables all interrupts by clearing the global interrupt mask.
	*/

	// Enable all interrupts
	sei();

	while (1)
	{
		blink();
	}
}
