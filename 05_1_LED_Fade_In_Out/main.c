/*
 * LED Fade In Fade Out. (Heartbeat) Hardware PWM
 * Created: 30.05.2015 14:48:31
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested: YES
 */

#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>

#define FADE_DELAY 10
#define CORRECTION 165

#define LED PB0

void initIO() {
  DDRB |= (1 << LED);
  PORTB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1);

  // Analog comparator OFF
  ACSR |= (1 << ACD);
}

void initPWM() {
  // Start timer T0. No prescaler
  // TCCR0B |= (1 << CS00);

  // Start timer T0. Prescaler 8
  // TCCR0B |= (1 << CS01);

  // Start timer T0. Prescaler 64
  TCCR0B |= (1 << CS01) | (1 << CS00);

  // Start timer T0. Prescaler 256
  // TCCR0B |= (1 << CS02);

  // Start timer T0. Prescaler 1024
  // TCCR0B |= (1 << CS02) | (1 << CS00);

  // Enable Fast PWM
  TCCR0A |= (1 << WGM01) | (1 << WGM00);

  /*
  Non-inverting Mode:
  COM0A1, COM0A0 [1,0] Clear OC0A on Compare Match, set OC0A at TOP
  Inverting Mode:
  COM01, COM00 [1,1] Set OC0A on Compare Match, clear OC0A at TOP
  */

  // Non-inverting Compare Output Mode
  TCCR0A |= (1 << COM0A1);

  // Reset timer T0 flags.
  TIFR0 = 0;
}

void fadeOut() {
  // Here could be a trap with <= and int8_t 1 byte var
  for (uint8_t i = 0; i < (255 - CORRECTION); i++) {
    OCR0A = i;
    _delay_ms(FADE_DELAY);
  }
}

void fadeIn() {
  for (uint8_t i = (255 - CORRECTION); i > 0; i--) {
    OCR0A = i;
    _delay_ms(FADE_DELAY);
  }
}

int main(void) {
  initIO();
  initPWM();

  while (1) {
    fadeOut();
    fadeIn();
  }

  return 0;
}

/*
# Comments:
===================================
## What does the program do?
Heartbeat. The LED's brightness will be adjusted based on
the value stored in the Output Compare Register (OCR0A).

Circuit connections:
PB0 - LED - 2,2k - GND Hardware PWM Output
PB1 - *
PB2 - *
PB3 - *
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

Output Compare Unit
------------------------
The 8-bit comparator continuously compares TCNT0 with the Output Compare
Registers OCR0A and OCR0B. The Waveform Generator uses the match signal to
generate an output according to operating mode.

Think about:
The Timer's Counter value TCNT0 is compared with the value stored in the
Output Compare Register OCR0A.

# Conclusions
===================================
Run this code and find what behavior is expected and what we got.
*/
