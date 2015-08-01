/*
 * Blinking_Duty_Ratio.c
 * LED 100 ms ON, 900 ms OFF.
 * Created: 01.08.2015 22:16:59
 * Author: LbookA
 */ 

#define F_CPU 1200000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int8_t status_led_flag=0;
uint16_t timer_counter=0;
volatile int8_t led_off_flag=0;
volatile int8_t led_on_flag=0;


#define MAX_TIMER_TIME 65535
#define BLINK_LED_ON_TIME	100
#define BLINK_LED_OFF_TIME	900

#define tog(x)	^= (1<<x)
#define set(x)	|= (1<<x)
#define clr(x)	&=~(1<<x)

#define LED PB2

ISR (TIM0_COMPA_vect) {
	
	timer_counter++;
	
	if (timer_counter >= BLINK_LED_OFF_TIME)
	{
		timer_counter=0;
	}
	
	if (timer_counter <= BLINK_LED_ON_TIME)
	{
		led_on_flag=1;
	}
	else
	{
		led_on_flag=0;
	}
	
}

void Setup() {
	
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

void Start_Blinking() {
	
	if (led_on_flag)
	{
		PORTB set(LED);
	}
	else
	{
		PORTB clr(LED);
	}
	
}

int main(void)
{
	Setup();
	sei();
	
	while(1)
	{
		Start_Blinking();	
	}
}



