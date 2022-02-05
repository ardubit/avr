/*
 * Duty Cycle of LED - 50 ms.
 * Created: 03.08.2015 9:48:12
 * Author: Aleksey M.
 * MCU ATtiny13
 */

/*
The duty cycle describes the amount of time the signal is in a high (on) state
as a percentage of the total time of it takes to complete one cycle.
*/

#define F_CPU 1200000UL
#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t timer_counter = 0;
volatile int8_t led_flag = 0;

int prev_duty_cycle, duty_cycle = 50;
char state = 0;

#define tog(x) ^= (1 << x)
#define set(x) |= (1 << x)
#define clr(x) &= ~(1 << x)

#define LED PB2

ISR(TIM0_COMPA_vect)
{
	timer_counter++;

	if ((timer_counter >= duty_cycle) && (!state))
	{
		timer_counter = 0;
		prev_duty_cycle = duty_cycle;
		int period = 1000;
		duty_cycle = period - duty_cycle;
		state = 1;
		led_flag = 1;
	}

	if ((timer_counter >= duty_cycle) && (state))
	{
		timer_counter = 0;
		duty_cycle = prev_duty_cycle;
		state = 0;
		led_flag = 0;
	}
}

void setup()
{

	// LEDs on the PB2, PB3
	DDRB = 0b00011101;
	PORTB = 0b00100010;

	// Analog comparator OFF
	ACSR |= (1 << ACD);

	// Start timer T0 with prescaler 8
	TCCR0B |= (1 << CS01);

	// Enable time T0 overflow interrupt
	TIMSK0 |= (1 << OCIE0A);

	// Enable CTC Mode.
	TCCR0A |= (1 << WGM01);

	// T0 will overflow each 1 ms
	OCR0A = 150;

	// Reset timer T0 flags
	TIFR0 = 0;
}

void start_Blink()
{
	if (led_flag)
	{
		PORTB clr(LED);
	}
	else
	{
		PORTB set(LED);
	}
}

int main(void)
{
	setup();
	sei();

	while (1)
	{
		start_Blink();
	}
}
