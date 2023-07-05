/*
 * Timer PWM. Hardware PWM (Pulse-Width Modulation)
 * Created: 30.05.2015 14:48:31
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested: YES
 */

#define F_CPU 1200000UL
#include <avr/interrupt.h>
#include <avr/io.h>

#define LED PB0

void initIO() {
  DDRB |= (1 << LED);
  PORTB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1);

  // Analog comparator OFF
  ACSR |= (1 << ACD);
}

void initPWM() {
  // Timer Setup
  // -------------------------
  // Start timer T0 with prescaler 64
  TCCR0B |= (1 << CS01) | (1 << CS00);

  // Enable Fast PWM
  TCCR0A |= (1 << WGM01) | (1 << WGM00);

  /*
  Non-inverting Mode:
  COM0A1, COM0A0 [1,0] Clear OC0A on Compare Match, set OC0A at TOP
  Inverting Mode:
  COM01, COM00 [1,1] Set OC0A on Compare Match, clear OC0A at TOP
  */

  // Non-inverting Compare Output Mode
  TCCR0A |= (1 << COM0A1) | (0 << COM0A0);

  // Reset timer T0 flags
  TIFR0 = 0;
}

int main(void) {
  initIO();
  initPWM();
  OCR0A = 250; // if TCNT0 == OCR0A, then OC0A pin will toggle

  while (1) {
    // Application code
  }

  return 0;
}

/*
# Comments:
===================================
## What does the program do?
Implementation of hardware PWM. The LED's brightness will be adjusted based on
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
Registers (OCR0A and OCR0B). The Waveform Generator uses the match signal to
generate an output according to operating mode.

Think about:
The Timer's Counter value (TCNT0) is compared with the value stored in the
Output Compare Register (OCR0A).

The Timer (TCNT0) and Output Compare Registers (OCR0A and OCR0B)
------------------------------------
When value in the TCNT0 == OCR0A CPU can generate compare match interrupt

// Compare Match A, B interrupt
// Uncomment to enable
// TIMSK0 |= (0<<OCIE0A) | (0<<OCIE0B);
// OCR0A,B compare register values
// OCR0A = 100;
// OCR0B = 200;

********** TIMER MODES: **********

1. Normal Mode
-------------------
Timer count up to [0 - 255]
- A new counter value can be written anytime.
- The Output Compare Unit can be used to generate interrupts at some given
time/value.
- Timer triggers interrupts.
Advantages:
- Time ticker
Disadvantages:
- Waveforms generation in Normal mode is not recommended, due it will occupy too
much of the CPU time

2. Clear Timer on Compare Match (CTC) Mode
-------------------
Timer count up to [0 - Compare Register] OCR0A
- OCR0A defines the top value.
- In CTC mode the counter is cleared to zero when the counter value TCNT0
matches the OCR0A.
- Timer trigger interrupts. Possible not to use interrupts and still toggle an
output pin.
- Parallel processing to the CPU and doesn't interrupt anything.
Advantages:
- Periods counting
- Interrupt generation via defined time

3. Fast PWM Mode:
-------------------
Timer count up to [0 - 255]
- In non-inverting Compare Output mode
if TCNT0 == OCR0A, then OC0A will toggle to "0"
when TCHT0 reach 255, then OC0A will toggle back to "1"
Advantages:
- PWM runs in the background like CTC.
- High frequency of PWM: rectification, and DAC applications

4. Phase Correct PWM Mode:
-------------------
Timer count up to [0 - 255, and then down 255 - 0]
PWM freq. is 1/2 * Fast PWM
Advantages:
- High resolution phase correct PWM waveform
- Centers of pulses are always matched
- Preferred for motor control applications

TODO
-------------------
- Add PWM output voltage calc

# Conclusions
===================================
Run this code and find what behavior is expected and what we got.
*/