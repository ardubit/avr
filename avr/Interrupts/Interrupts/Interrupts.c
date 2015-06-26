/*
 * Interrupts.c
 * External INT0 Interrupt. 
 * Created: 24.05.2015 1:28:05
 * Author: LbookA
 * MCU ATtiny13
 */ 

#define F_CPU 1200000UL
#include <avr/io.h>

// Interrupt library.
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char int0_flag=0;

// Here is an example when we execute two tasks: blinking LED_1 on BP2 and toggle LED_2 on BP3 when interrupt is occur.
// It may seem that everything is happening at once, but it's not true. Everything is going one by one.
// Interruption is a way to implement multitasking.

// This is an interrupt vector "External Interrupt Request 0". All vectors look at the table 9-1, page 45.
// When the interrupt occur, this code will execute.
ISR (INT0_vect) {
	
	// For hardware debounce reduction add in parallel to the button a capacitor with 100n of value.
	PORTB ^= (1<<PB3);
	
	/* ------- Simple software debounce without _delay_ms() function | Start ------- */
	
	// Delays in the interrupt vectors is not a good idea. 
	// Because when microcontroller wait a delay time and doing nothing, could occur another interrupts and you will lose event or data.
	// I recommend leave a capacitor too.
	
	/* 
	
	if (!(PINB & (1<<PB1)) && (int0_flag == 0))
	{
		int0_flag = 1;
		
		 // Toggle LED.
		 // "^" Binary XOR Operator copies the bit if it is set in one operand but not both.
		PORTB ^= (1<<PB3);
	}
	
	int0_flag=0;
	
	// For avoid a repeat execution clean up (before exit from vector) the flag setting logical one in Bit 6, GIFR – General Interrupt Flag Register
	// The flag is cleared when the interrupt routine is executed. Page 48.
	GIFR |= (1<<INTF0);
	
	*/
	
	/* ------- Software debounce without delay function | End ------- */
	
}

void setup () {
	DDRB =  0b00011101;
	PORTB = 0b00100010;
	
	// General Interrupt Mask Register. The bit 6 turn ON Interrupt Request on INT0 pin of microcontroller.
	GIMSK =	(1<<INT0);
	
	// MCU Control Register contains control bits for interrupt sense control.
	// The falling edge of INT0 generates an interrupt request. All settings look at the table 9-2, page 47.
	MCUCR |= (1<<ISC01);
	MCUCR &=~(1<<ISC00);
}

void blink () {
	PORTB ^= (1<<PB2);
	_delay_ms(500);
}

int main(void)
{
	setup ();
	
	// The interrupts should globally enabled in the code. By default, interrupts are globally switched off.
	// The global interrupt flag is maintained in the "I" bit of the status register (SREG). Page 9.
	// cli(); disables all interrupts by clearing the global interrupt mask. 
	sei ();
	
    while(1)
    {
        blink();
    }
}
