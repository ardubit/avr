/*
 * Simultaneously generates a PWM output with the blinking of an LED
 * Created: 30.05.2015 14:48:31
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested: NO
 */

#define F_CPU 1200000UL
#include <avr/interrupt.h>
#include <avr/io.h>

#define LED1 PB0
#define LED2 PB2
#define RST PB5

#define LED1_ON PORTB = PORTB | (1 << LED1)
#define LED1_OFF PORTB = PORTB & ~(1 << LED1)
#define LED1_TOG PORTB ^= (1 << LED1)

#define LED2_ON PORTB = PORTB | (1 << LED2)
#define LED2_OFF PORTB = PORTB & ~(1 << LED2)
#define LED2_TOG PORTB ^= (1 << LED2)

#define PRESCALE_1024 (1 << CS00) | (1 << CS02)
#define PRESCALE_NO (1 << CS00)

void initIO() {
  DDRB |= (1 << PB2) | (1 << PB1) | (1 << PB0);
  PORTB |= (1 << RST) | (1 << PB4) | (1 << PB3);

  // Analog comparator OFF
  ACSR |= (1 << ACD);
}

void initTimer() {
  TCCR0B |= PRESCALE_NO;
  TIMSK0 |= (1 << TOIE0) | (1 << OCIE0A) | (1 << OCIE0B);

  OCR0A = 150;
  OCR0B = 150;

  // CTC
  TCCR0A |= (1 << WGM01);

  // OCR0A
  TCCR0A |= (1 << COM0A0);

  // OCR0B
  TCCR0A |= (1 << COM0B0);
}

void blink() {
  volatile static uint8_t a = 0;
  a++;
  if (a == 150) {
    LED2_TOG;
    a = 0;
  }
}

ISR(TIM0_OVF_vect) { blink(); }

// OCR0A
ISR(TIM0_COMPA_vect) { asm("nop"); }

// OCR0B
ISR(TIM0_COMPB_vect) { asm("nop"); }

int main(void) {
  initIO();
  initTimer();
  sei();

  while (1) {
    asm("nop");
  }

  return 0;
}

/*
# Comments:
===================================
## What does the program do?
CTC Clear Timer on Compare Match (CTC) Mode. Generate a PWM output while blinking an LED.

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

Output Compare Unit
------------------------
The 8-bit comparator continuously compares TCNT0 with the Output Compare
Registers OCR0A and OCR0B. The Waveform Generator uses the match signal to
generate an output according to operating mode. The Timer's Counter value TCNT0
is compared with the value stored in the Output Compare Register OCR0A.

# Conclusion
===================================
Run this code and find what behavior is expected and what we got.
*/
