/*
 * Blinking LED - 1 Hz
 * Created: 23.05.2015 21:19:05
 * Author: Aleksey M.
 * MCU Atmel ATtiny13
 * Tested: YES
 */

// MCU Frequency - 1,2 MHz. 9,6 MHz default clock with prescaling of 8, page 26
#define F_CPU 1200000UL
#define LED PB3

#include <avr/io.h>     // IO definition library for work with registers names
#include <util/delay.h> // The delay library requires to define F_CPU

int main(void) {
  // Port B Data Direction Register. Set Input/Output func for pin
  DDRB = 0b00001000;  // 0x8 HEX

  // Port B Data Register
  PORTB = 0b00110111; // 0x37 

  while (1) {
    PORTB ^= (1 << LED); // Toggle bit LED ON/OF
    _delay_ms(500);

    // Classic "Hello World" program
    // PORTB |= (1<<PB2);
    //_delay_ms (500);
    // PORTB &= ~(1<<PB2);
    //_delay_ms (500);
  }

  // Should it return something?
  return 0;
}

/*
# Comments:
===================================
## What does the program do?
It is a classic "hello world" program for microcontrollers, frequently referred
to as the 'blink' program.

Circuit connections:
DDRB. Define direction for port pins
'0' - input, '1' - output, unused '*' pins config to 'inputs' with enabled
internal pull-up resistors

PORT. Define pullups resistors to "VCC" for inputs or logic states for outputs
Input: '1' - pullup enable, '0' - pullup disable
Output: '1' - logic one, '0' - logic zero

PB0 - Button - GND
PB1 - *
PB2	- *
PB3 - LED - 2,2k - GND
PB4 - *
PB5 - Reset (input)
PB6, PB7 are Read-only bits

DC Current per I/O Pin 40.0mA
LEDred Vf = 1,65V, I = 1,5mA
VCC = 5V
R = (5V - 1,65V) / 2 = 2,23K = 2,2K

## Notes:
f = 1/T, T = 1 sec

The RESET operation initializes all I/O registers and sets
the Program Counter (PC) to zero.

# Conclusions
===================================
Run this code and find what behavior is expected and what we got.
*/
