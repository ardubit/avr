/*
 * LED - ON 100 ms, OFF - 900 ms
 * Created: 01.08.2015 22:16:59
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested: YES
 */

#define F_CPU 1200000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

volatile uint16_t timerCtr = 0;
volatile bool flagLED = false;

#define LED_ON_TIME 100
#define LED_OFF_TIME 900

// Macros
#define tog(x) ^= (1 << x)
#define set(x) |= (1 << x)
#define clr(x) &= ~(1 << x)

#define LED PB3

void initIO() {
  DDRB |= (1 << LED);
  PORTB |= (1 << PB5) | (1 << PB4) | (1 << PB2) | (1 << PB1) | (1 << PB0);

  // Analog comparator OFF
  ACSR |= (1 << ACD);
}

void initTimer() {
  // Start timer T0 with prescaler 8
  TCCR0B |= (1 << CS01);

  // Enable CTC Mode
  TCCR0A |= (1 << WGM01);

  /*
  In the CTC mode the counter is cleared to zero when the counter value
  TCNT0 matches the OCR0A
  */

  // T0 will overflow each 1 ms (0,001 sec)
  OCR0A = 150;

  // Enable time T0 overflow interrupt
  TIMSK0 |= (1 << OCIE0A);

  // Reset timer T0 flags
  TIFR0 = 0;
}

void blink() {
  if (flagLED) {
    PORTB set(LED);
  } else {
    PORTB clr(LED);
  }
}

// Timer/counter Compare Match A Vector. 1 ms
ISR(TIM0_COMPA_vect) {
  timerCtr++;
  if (timerCtr >= LED_OFF_TIME) {
    timerCtr = 0;
  }

  if (timerCtr <= LED_ON_TIME) {
    flagLED = 1;
  } else {
    flagLED = 0;
  }
}

int main(void) {
  initIO();
  initTimer();
  sei();

  while (1) {
    blink();
  }

  return 0;
}

/*
# Comments:
===================================
## What does the program do?
The duty cycle describes the amount of time the signal is in a HIGH (on) state
as a percentage of the total time of it takes to complete one cycle. Full cycle
- 1s, HIGH - 100 ms, LOW - 900 ms

Circuit connections:
PB0 - *
PB1 - *
PB2 - *
PB3 - LED - 2,2k - GND
PB4 - *
PB5 - Reset (input)
For hardware debounce reduction add in parallel with the button 100nf
capacitor.

## Notes:
Timer Setup
---------------------------------------
- Timer T0 (8-bit) counts up to or bi-directional 2^8 - 1 = 255 then timer's
register TCNT0 will overflow.
- Timer clock frequency equal to system clock frequency = 1,2 MHz (or by
external clock source).
- Prescaler can be used as a clock source: fCLK_io/8, fCLK_io/64, fCLK_io/256,
or fCLK_io/1024.

Timer Clock calculations:
---------------------------------------------
1,2 MHz / 8 = 0,15 MHz = 150 KHz timer's clock, where 8 is prescaler
150 KHz or increment per second (incr/s)
Timer increments by +1 every:
1/150 = 0,006666666666667 ms

Let's take 1 ms:
Once 1 ms has elapsed, timer T0 will overflow and trigger an interrupt, which
the MCU will then handle by executing the Timer's ISR routine. 0,006666666666667
ms	-> 1 incr
1 ms 									-> x
x = 1/0,006666666666667 = 150

Timer Clock calculations #2:
------------------------------------
Number of overflows = F timer / timer's MAX count
150KHz / 150 = 1 overflow per ms

The Timer (TCNT0) and Output Compare Registers (OCR0A and OCR0B)
------------------------------------
When value in the TCNT0 == OCR0A CPU can generate compare match interrupt

// Compare Match A, B interrupt
// Uncomment to enable
// TIMSK0 |= (0<<OCIE0A) | (0<<OCIE0B);
// OCR0A,B compare register values
// OCR0A = 100;
// OCR0B = 200;

# Conclusions
===================================
Run this code and find what behavior is expected and what we got.
*/
