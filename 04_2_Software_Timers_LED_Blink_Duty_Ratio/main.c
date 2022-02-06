/*
 * LED - ON 100 ms, OFF - 900 ms
 * Created: 01.08.2015 22:16:59
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested
 */

/*
The duty cycle describes the amount of time the signal is in a HIGH (on) state
as a percentage of the total time of it takes to complete one cycle.

Full cycle - 1s, HIGH - 100 ms, LOW - 900 ms
*/

#define F_CPU 1200000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

volatile uint16_t timer_counter = 0;
volatile bool led_flag = false;

#define LED_ON_TIME 100
#define LED_OFF_TIME 900

// Macros
#define tog(x) ^= (1 << x)
#define set(x) |= (1 << x)
#define clr(x) &= ~(1 << x)

#define LED PB3

/*
- Timer T0 (8-bit) counts up to (or bi-directional) – (2^8) - 1 = 255
- Timer clock frequency equal to system clock frequency = 1,2 MHz (or by external clock source)
- Prescaler can be used as a clock source: fCLK_io/8, fCLK_io/64, fCLK_io/256, or fCLK_io/1024
*/

// 1 ms
// Timer/Counter Compare Match A Vector
ISR(TIM0_COMPA_vect)
{
	timer_counter++;
	if (timer_counter >= LED_OFF_TIME)
	{
		timer_counter = 0;
	}

	if (timer_counter <= LED_ON_TIME)
	{
		led_flag = 1;
	}
	else
	{
		led_flag = 0;
	}
}

void setup()
{
	/*
	PB0, PB1, PB2 - *
	PB3 - LED - 2,2k - GND
	PB4 - *
	PB5 - Reset (input)
	*/

	// Always set all bits!
	DDRB |= (0 << PB5) | (0 << PB4) | (1 << PB3) | (0 << PB2) | (0 << PB1) | (0 << PB0);
	PORTB |= (1 << PB5) | (1 << PB4) | (0 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);

	// Analog comparator OFF
	ACSR |= (1 << ACD);

	// Start timer T0 with prescaler 8
	TCCR0B |= (1 << CS01);

	/*
	Timer Clock calculations:
	---------------------------------------------
	1,2 MHz / 8 = 0,15 MHz = 150 KHz timer's clock
	150 KHz or increment per second (incr/s)

	Timer increments by +1 every:
	1/150 = 0,006666666666667 ms

	Let's take 1 ms:
	- After 1 ms timer T0 will overflow
	- Generate an interrupt 
	- MCU will go to the Timer's ISR routine

	0,006666666666667 ms	-> 1 incr
	1 ms 									-> x
	x = 1/0,006666666666667 = 150

	Timer Clock calculations Second Way:
	---------------------------------------------
	Number of overflows = F timer / timer's MAX count
	150KHz / 150 = 1 overflow per ms.
	*/

	// Enable CTC Mode
	TCCR0A |= (1 << WGM01);

	/*
	In CTC mode the counter is cleared to zero when the counter value
	TCNT0 matches the OCR0A
	*/

	// T0 will overflow each 1 ms (0,001 sec)
	OCR0A = 150;

	// Enable time T0 overflow interrupt
	TIMSK0 |= (1 << OCIE0A);

	// Reset timer T0 flags
	TIFR0 = 0;
}

void start_Blink()
{
	if (led_flag)
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
	setup();
	sei();
	while (1)
	{
		start_Blink();
	}
}
