/*
 * Button. Press the button to activate the blinking LED
 * Created: 23.05.2015 23:14:09
 * Author: Aleksey M.
 * MCU Atmel ATtiny13
 * Tested: YES
 */

#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>

#define LED_ON PORTB |= (1 << PB3)  // Set HI
#define LED_OF PORTB &= ~(1 << PB3) // Set LO

// Configuration
void initIO() {
  DDRB = 0x8;
  PORTB = 0x37;
}

int main(void) {
  initIO();

  while (1) {
    if (!(PINB & (1 << PB0))) {
      LED_ON;
      _delay_ms(250);
      LED_OF;
      _delay_ms(250);
    }
  }

  return 0;
}

/*
# Comments:
===================================
## What does the program do?
It is a classic the button activated "hello world" program for microcontrollers.

Circuit connections:
Define direction for port pins
'0' - input, '1' - output, unused '*' pins config to 'inputs' with enabled
internal pull-up resistors

PB0 - Button - GND
PB1 - *
PB2	- *
PB3 - LED - 2,2k - GND
PB4 - *
PB5 - Reset (input)
PB6, PB7 are Read-only bits

## Notes:
Here is another way that makes code more readable and simple. The C Preprocessor
will replace #define LED_ON with the value of PORTB |= (1<<PB0) when compilation
is started.

The button is on PB0 with an internal pullup resistor to VCC, when a button is
pressed a logic level will change from 1 to 0. Conditions in parentheses of the
function require a logic "true" for executing code in the "{}".

The (PINB & (1<<PB1)) equal to '1' as logic 'true' if a button is not pressed
and that's why we should use "!" to reverse it.

AND operation:
PINB 			1010 1110 (Let's assume that represents the register)
(1<<PB1)	0000 0010
          ---------
BTN       0000 0010 '> 1'

AND operation:
PINB 			1010 1100
(1<<PB1)	0000 0010
          ---------
BTN       0000 0000 '0'

# Conclusions
===================================
Run this code and find what behavior is expected and what we got.
*/
