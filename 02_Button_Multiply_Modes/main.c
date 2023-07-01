/*
 * Button with LEDs
 * Created: 26.06.2015 1:34:02
 * Author: Aleksey M.
 * MCU Atmel ATtiny13
 * Tested: YES
 */

#define F_CPU 1200000UL
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

// 1 byte of memory
unsigned char ctr = 0; // It's better to use uint8_t for portable code
bool latch = false;

#define BTN PB0
#define UNU1 PB1 // Unused pin
#define UNU2 PB2
#define LED1 PB3
#define LED2 PB4
#define RST PB5

void initIO() {
  // Set outputs
  DDRB = DDRB | (1 << LED1) | (1 << LED2) ;
  // Pullup them
  PORTB = PORTB | (1 << BTN) | (1 << UNU1) | (1 << UNU2) | (1 << RST); 
}

int main(void) {
  initIO();

  while (1) {
    // Verify whether the button has been pressed
    if (!(PINB & (1 << BTN))) {
      _delay_ms(20); // Debounce the btn flickering and check again

      if (!(PINB & (1 << BTN)) && (latch == false)) {
        // Latch to avoid re-entry into the function until the button is
        // pressed. Microcontroller is very fast!
        latch = true;
        ctr++;

        // When limit of modes is reached reset the counter. Turn LEDs 'off'
        if (ctr > 2) {
          ctr = 0;
          PORTB &= ~((1 << LED1) | (1 << LED2));

          // The same as:
          // PORTB &= ~(1<<PB2);
          // PORTB &= ~(1<<PB3);
        }
      }
    }

    // When btn is unpressed turn back the latch
    if ((PINB & (1 << BTN)) && (latch == false)) {
      latch = true;
    }

    // Mode 1
    if (ctr == 1) {
      PORTB |= (1 << LED1);
    }

    // Mode 2
    if (ctr == 2) {
      PORTB |= (1 << LED2);
    }
  }

  return 0;
}

/*
# Comments:
===================================
## What does the program do?
It counts button pushes with LEDs and then reset them.

Circuit connections:
PB0 - Button - GND
PB1 - *
PB2	- *
PB3 - LED - 2,2k - GND
PB4 - LED - 2,2k - GND
PB5 - Reset (input)
PB6, PB7 are Read-only bits

## Notes:
OR operationis for SET bit:
PORTB 		1010 1100 (Let's assume that represents the register)
(1<<LED1)	0000 0010
          ---------
BTN       1010 1110 '> 1'
1-st bit is set other bits are untouched

# Conclusions
===================================
Run this code and find what behavior is expected and what we got.
*/
