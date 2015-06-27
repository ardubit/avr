/*
 * Double_Click.c
 * One click toggle LED_1, two clicks toggle LED_2.
 * Created: 27.06.2015 12:05:19
 * Author: LbookA
 * MCU Tiny13
 */ 

#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned int timer_counter=0;
unsigned int t2=0;
unsigned char latch=0, pulse_counter=0;
unsigned char one_click_state=0; 
unsigned char double_click_state=0;

#define max_double_click 400 // ms. Usually a human reaction is near 300-350 ms.

ISR (TIM0_COMPA_vect) {
	
	timer_counter++;
	if (timer_counter == max_double_click)
	{
		
		timer_counter=0;
		
		if (pulse_counter == 1)
		{
			pulse_counter=0;
			one_click_state=1;
		}
		
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

void one_click () {
	PORTB ^= (1<<PB2);
}

void double_click () {
	PORTB ^= (1<<PB3);
}

void check_button () {
	
	if (!(PINB & (1<<PB1)))
	{
		_delay_ms(20);
			
			if (!(PINB & (1<<PB1)) && (latch == 0))
			{
				latch=1;
				pulse_counter++;
			
				if (pulse_counter == 1)
				{
					timer_counter=0;
					t2=0;
				}
		
				if (pulse_counter == 2)
				{
					pulse_counter=0;
					t2=timer_counter;
				}
			}
	}
	
	if ((PINB & (1<<PB1)) && (latch == 1))
	{
		latch=0;
	}
	
}

void double_click_capture () {

	if ((t2 <= max_double_click) && (t2 != 0))
	{
		double_click_state=1;
		t2=0;
	}

}

int main(void)
{
	
	setup();
	sei();
	
	while(1)
	{
		check_button();
		double_click_capture();
		
		if (double_click_state == 1)
		{
			double_click();
			double_click_state = 0;
		}
		
		
		if (one_click_state == 1)
		{
			one_click();
			one_click_state = 0;
		}
	}
}