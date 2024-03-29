/*
 * Timer Interrupts. LED toggle in the Timer's ISR
 * Created: 24.05.2015 16:55:36
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested: YES
 */

#define F_CPU 1200000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define LED1 PB3
#define LED2 PB4
#define RST PB5

#define LED1_ON PORTB = PORTB | (1 << LED1)
#define LED1_OFF PORTB = PORTB & ~(1 << LED1)
#define LED2_ON PORTB = PORTB | (1 << LED2)
#define LED2_OFF PORTB = PORTB & ~(1 << LED2)
#define LED1_TOG PORTB ^= (1 << LED1)
#define LED2_TOG PORTB ^= (1 << LED2)

// Timer/counter overflow handler
ISR(TIM0_OVF_vect) {
  LED1_TOG;
  _delay_ms(50); // Only for debug. Do not use delays in the ISRs!
}

// Timer/counter Compare Match A - OCR0A
ISR(TIM0_COMPA_vect) {
  asm("nop"); // Do nothing
}

// Timer/counter Compare Match B - OCR0B
ISR(TIM0_COMPB_vect) {
  asm("nop"); // Do nothing
}

void initIO() {
  DDRB |= (1 << LED1) | (1 << LED2);
  PORTB |= (1 << RST) | (1 << PB2) | (1 << PB1) | (1 << PB0);

  // Analog comparator OFF
  ACSR |= (1 << ACD);
}

void initTimer() {
  // Normal mode
  TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0));

  // Start timer T0 with No prescaling
  TCCR0B |= (1 << CS00);

  // Timer/counter interrupt mask reg. Overflow interrupt enable
  TIMSK0 |= (1 << TOIE0);

  // Reset timer T0 flags
  TIFR0 = 0;
}

int main(void) {
  cli(); // Clear the global interrupt flag
  initIO();
  initTimer();
  sei(); // Enable global interrupts

  while (1) {
    asm("nop");
  }

  return 0;
}

/*
# Comments:
===================================
## What does the program do?
When timer interrupts occur, the LED toggles inside the timer's Interrupt
Service Routine (ISR).

Circuit connections:
PB0 - *
PB1 - *
PB2 - *
PB3 - LED - 2,2k - GND
PB4 - LED - 2,2k - GND
PB5 - Reset (input)
For hardware debounce reduction add in parallel with the button 100nf
capacitor.

## Notes:
Timer Setup
---------------------------------------
Timer T0 (8-bit) counts up to or bi-directional 2^8 - 1 = 255 then timer's
register TCNT0 will overflow.

  TCNT0 0-255
    overflow at TOP
       /|    /|
      / |   / |
OCR0 x  |  x  | <--- MCU compare TCNT0 with OCR0 value and then generate ISR
    /   | /   |
   /    |/    |

  TCNT0 0-255-0
    overflow
       /\
      /  \
OCR0 x    x
    /      \
   /        \

Timer/Counter Control Register A
---------------------------------------
- Timer mode: Normal port operation
Normal Mode set by default OC0A disconnected
COM0A1, COM0A0 – [0,0]
- Waveform generation

The Timer (TCNT0) and Output Compare Registers (OCR0A and OCR0B)
---------------------------------------
When value in the TCNT0 == OCR0A CPU can generate compare match interrupt

// Compare Match A, B interrupt
// Uncomment to enable
// TIMSK0 |= (0<<OCIE0A) | (0<<OCIE0B);
// OCR0A,B compare register values
// OCR0A = 100;
// OCR0B = 200;

## About 'asm("nop")'
if asm("nop") isn't recognized by compiler
Edit make file to: option -std=c99 to -std=gnu99

For c99 mode, the compiler does not recognise 'asm', '__asm' ist required
instead! For gnu99 mode, both variants are accepted, 'asm' and '__asm' as
well!

# Conclusions
===================================
Run this code and find what behavior is expected and what we got.
*/
