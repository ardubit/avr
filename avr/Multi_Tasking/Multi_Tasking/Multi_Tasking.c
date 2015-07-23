/*
 * Multi_Tasking.c
 * Blinking LED_1 without _delay_ms function with Button handling which turn on/off LED_2.
 * Created: 22.07.2015 19:49:07
 * Author: LbookA
 * MCU Tiny13
 */ 

#define F_CPU 1200000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

volatile uint16_t timer_counter=0;
int8_t flag=0;

#define BLINK_LED 500

ISR (TIM0_COMPA_vect) {
	
	timer_counter++;
	
	
	if (timer_counter >= BLINK_LED)
	{
		timer_counter=0;
		flag=1;
	}
	
}

void setup () {
	
	// LEDs on PB2, PB3.
	DDRB =  0b00011101;
	PORTB = 0b00100010;
	
	// Analog comparator OFF.
	ACSR |= (1 << ACD);
	
	// Start timer T0 with prescaler 8.
	TCCR0B |= (1<<CS01);
	
	// Enable time T0 overflow interrupt.
	TIMSK0 |= (1<<OCIE0A);
	
	// Enable CTC Mode.
	TCCR0A |= (1<<WGM01);
	
	// T0 will overflow each 1 ms.
	OCR0A = 150;
	
	// Reset timer T0 flags.
	TIFR0 = 0;
	
}

void Blinking () {
	 
	 if (flag) 
	 {
		 // Toggle LED.
		 PORTB ^= (1<<PB3);
		 
		 // Interrupt protection before accessing variable.
		 ATOMIC_BLOCK(ATOMIC_FORCEON)
		 {
			 flag=0;
		 }
		 
	 }
	
}

int main(void)
{
	setup ();
	sei();
	
    while(1)
    {
       
		Blinking();
		
		if (!(PINB & (1<<PB1)))
		{
			PORTB |= (1<<PB2);
		}
		else
		{
			PORTB &=~(1<<PB2);
		}
		
    }
	
}
