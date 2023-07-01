/*
 * External PCINT0-5 interrupt. Button interrupt
 * Created: 01.07.2023 12:13:05
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested: YES
 */

#define F_CPU 1200000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/atomic.h>
#include <util/delay.h>

#define LED PB2
#define BTN PB4
#define RST PB5

#define LED_ON PORTB |= (1 << LED)
#define LED_OF PORTB &= ~(1 << LED)
#define LED_TG PORTB ^= (1 << LED)

#define BTN_LO (!(PINB & (1 << BTN)))
#define BTN_HI (PINB & (1 << BTN))

volatile uint8_t ctr = 0; // remove 'volatile' and do conclusions
uint8_t prevCtr = 0;

volatile bool fA = true;
volatile bool fB = true;

void initIO() {
  // Set LED as output
  DDRB |= (1 << LED);

  // Set BTN as input with pull-up resistor enabled
  DDRB &= ~(1 << BTN); // 0
  PORTB |= (1 << BTN); // 1

  // Set RST as input
  DDRB &= ~(1 << RST);
  PORTB |= (1 << RST);
}

void initExternIS() {
  // Pin change interrupt is enabled on the corresponding IO pin. Any change on
  // any enabled PCINT5..0 pin will cause an interrupt, page 46, 47
  GIMSK |= (1 << PCIE);
  PCMSK |= (1 << PCINT4);
}

void blink() {
  PORTB ^= (1 << LED);
  _delay_ms(500); // it's a blocking call
}

void pulse() {
  LED_ON;
  _delay_ms(50); // it's a blocking call
  LED_OF;
  _delay_ms(300);
}

ISR(PCINT0_vect) {
  if (BTN_LO && fA) {
    fA = false; // Lock the door
    ctr = ctr & 0x3; // Not more than 3 + 1
    ctr++;      // Just +1 per each falling edge interrupt
  }

  if (BTN_HI && fB) {
    fB = false;
    // code for each rising edge interrupt
  }
}

int main(void) {
  initIO();
  initExternIS();
  sei();

  while (1) {
    // Main tasks here
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
      // Atomic code
    }

    if (ctr != prevCtr) {
      prevCtr = ctr;
      for (uint8_t i = 0; i < ctr; i++) {
        pulse();
      }
    }

    if (BTN_HI) {
      fA = true; // Unlock falling edge handler
    }
  }

  return 0;
}

/*
# Comments:
===================================

## What does the program do?
MCU executes two tasks: one in the main routine and serves the button interrupt
by FALLING and RASING edges. It increments the counter in the interrupt routine
and blinks the same times as the counter's value.

Circuit connections:
PB0 - *
PB1 - *
PB2 - LED - 2,2k - GND
PB3 - *
PB4 - Button - GND (PCINT4)
PB5 - Reset (input)
For hardware debounce reduction add in parallel with the button 100nf capacitor.

## Notes:
The term "Atomic" indicates that the respective code cannot be interrupted.

## About 'volatile'
If a variable can be changed out of compiler scope at any time in
interrupt ISR for example by button it should be processed without any
optimization and marked as 'volatile'. Global variables modified in
interrupt handler also should be marked as 'volatile'.

# Conclusions
===================================
Run this code and find what behavior is expected and what we got.
*/
