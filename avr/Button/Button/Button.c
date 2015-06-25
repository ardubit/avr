/*
 * Button.c
 * Simple button. When you press the button led will blinking.
 * Created: 23.05.2015 23:14:09
 * Author: Aleksey's Lbook
 * MCU Atmel ATtiny13
 *
 *
 */

#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>

// Here is another code way that make life more simple.
// The compiler will replace LED_ON with the value PORTB |= (1<<PB0) at compile time.
#define LED_ON PORTB |= (1<<PB2)
#define LED_OFF PORTB &= ~(1<<PB2)

// Simple configuration function.
void setup () {
	DDRB =  0b00011101;
	PORTB = 0b00100010;	
}

int main(void)
{
	// Let's run our configuration function here.
    setup ();
	
	while(1)
    {
	
	// Button is on PB1 with internal pullup resistor to VCC, when we press the button a logic level will changed from 1 to 0.
	// Conditions in parentheses of the function require a logic "True" for executing code in the "{}". 
	// (PINB & (1<<PB1) gives "1" and it's a logic true, but button not pressed that's why we use "!" to reverse it.
	if (!(PINB & (1<<PB1)))
        {
			LED_ON; // This is the place where #define works.
			_delay_ms(250);
			LED_OFF;
			_delay_ms(250);
        }		
			
    }
}