/*
 * Blinking LED
 * Created: 06.07.2023 00:42:05
 * Author: Aleksey M.
 * MCU Atmel ATtiny24
 * Tested: YES
 */

// MCU Frequency - 1,0 MHz, page 36
#define F_CPU 1000000UL
#define LED PA0
#define RST PB3

#include <avr/io.h>
#include <avr/iotn24.h> // IO definition library for work with registers names
#include <util/delay.h> // The delay library requires to define F_CPU

int main(void) {
  DDRA = (1 << LED);
  PORTB |= (1 << RST);

  while (1) {
    PORTA ^= (1 << LED);
    _delay_ms(100);
  }

  return 0;
}

/*
# Comments:
===================================
## What does the program do?
It is a classic "hello world" program for microcontrollers, frequently referred
to as the 'blink' program.

Circuit connections:
DDRA-B. Define direction for port pins
'0' - input, '1' - output,
unused '*' pins config to 'inputs' with enabled internal pull-up resistor

PORT. Define pullups resistors to "VCC" for inputs or logic states for outputs
Input: '1' - pullup enable, '0' - pullup disable
Output: '1' - logic one, '0' - logic zero

PB0 - *
PB1 - *
PB2	- *
PB3 - Reset (input)

PA0 - LED - 2,2k - GND
PA1 - *
PA2 - *
PA3 - *
PA4 - USCK
PA5 - MISO
PA6 - MOSI
PA7 - *

DC Current per I/O Pin 40.0mA
LEDred Vf = 1,65V, I = 1,5mA
VCC = 5V, R = (5V - 1,65V) / 2 = 2,23K = 2,2K

The RESET operation initializes all I/O registers and sets
the Program Counter (PC) to zero.

# Conclusions
===================================
Run this code and find what behavior is expected and what we got.
*/
