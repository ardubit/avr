/*
 * External INT0 interrupt. Button interrupt
 * Created: 24.05.2015 1:28:05
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested
 */

#define F_CPU 1200000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define BTN PB1
#define LED1 PB3
#define LED2 PB4
#define RST PB5

// Interrupt handler
ISR(INT0_vect) {
  PORTB |= (1 << PB3);
  _delay_ms(50); // It's only for debug. Do not use delays in the ISRs!
  PORTB = PORTB & ~(1 << PB3);
}

void initIO() {
  DDRB |= (1 << LED2) | (1 << LED1);
  PORTB |= (1 << BTN) | (1 << RST);
}

void initExternIS() {
  // Turn ON interrupt request on INT0 pin
  GIMSK |= (1 << INT0);

  // The falling edge of INT0 generates an interrupt request
  MCUCR |= (1 << ISC01);  // Set bit to '1'
  MCUCR &= ~(1 << ISC00); // Clear bit to '0'
}

void blink() {
  PORTB ^= (1 << PB4);
  _delay_ms(500);
}

int main(void) {
  initIO();
  initExternIS();
  sei(); // Enable all interrupts

  while (1) {
    blink();
  }

  return 0;
}

/*
# Comments:
===================================
## What does the program do?
The MCU performs two tasks: blinking LED1 and when an interrupt occurs it is
toggling LED2. It may seem that everything is happening at once, but it's not
true. Everything is going one by one. Interrupts are a way to implement
multitasking.

Circuit connections:
PB0 - *
PB1 - Button - GND (INT0)
PB2	- *
PB3 - LED - 2,2k - GND
PB4 - LED - 2,2k - GND
PB5 - Reset (input)
PB6, PB7 are Read-only bits
For hardware debounce reduction add in parallel with the button 100nf capacitor.

## Notes:
'INT0_vect' is an interrupt vector – "External Interrupt Request 0". For all
vectors look at the table 9-1, page 4. MCU Control Register contains bits for
interrupt sense control. All of the settings look at the table 9-2, page 47.

Interrupts should be globally enabled in code. By default, interrupts are
disabled. The global interrupt flag is maintained in the "I" bit of the status
register SREG, page 9. The cli() disables all interrupts by clearing the global
interrupt mask, sei() enable all interrupts.

# Conclusions
===================================
Run this code and find what behavior is expected and what we got.
*/

/*
Simple SOFTWARE DEBOUNCE without _delay_ms() function | Start
------------------------
Delays in the interrupt vectors are not a good idea and _delay_ms() is a
blocking function as well. It means that the microcontroller doing nothing when
executing this call. Another interrupt could occur anytime thus microcontroller
definitely will lose events or data.

volatile bool int0_flag = 0;

if (!(PINB & (1<<PB1)) && (int0_flag == 0))
{
        int0_flag = 1;
         // Toggle LED
        PORTB ^= (1<<PB3);
}

int0_flag = 0;

To avoid a repeat execution clean up before exit from vector in the
GIFR register flag. Set a logical '1' into bit 6, GIFR General Interrupt Flag
Register. The flag is cleared when the interrupt routine is executed, page 48

GIFR |= (1<<INTF0);
*/