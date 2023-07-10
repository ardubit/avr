/*
 * Analog to Digital converter. ADC
 * Created: 10.07.2023
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested: YES
 */

#include <avr/io.h>
#include <avr/iotn13.h>
#include <util/delay.h>

// MCU Frequency - 1,2 MHz. 9,6 MHz default clock with prescaling of 8, page 26
#define F_CPU 1200000UL

#define RESET_PIN PB5
#define LED PB0
#define ANALOGPOT PB3

#define PIN_TOG (PORTB ^= (1 << LED))

void initIO() {
  // DDRB '0' - input, '1' - output
  // Pullups resistors to 'VCC' or output logic states of pins
  // Input '1' - pullup enable, '0' - pullup disable

  // Reset pin as input
  DDRB &= ~(1 << RESET_PIN);
  PORTB |= (1 << RESET_PIN);

  // Set pin as output
  DDRB |= (1 << LED);

  // Not necessary
  DDRB &= ~(1 << ANALOGPOT); // Set to 0
  DIDR0 |= (1 << ANALOGPOT); // Disable digital input
}

void initADC() {
  // Set the ADC input to PB3/ADC3
  ADMUX |= (1 << MUX0) | (1 << MUX1);

  // Left adjust result
  ADMUX |= (1 << ADLAR);

  // Set the prescaler to clock/16 (1.2Mhz/16 = 75Khz)
  ADCSRA |= (1 << ADPS2);

  // ADC Enable
  ADCSRA |= (1 << ADEN);
}

uint8_t getADC() {
  ADCSRA |= (1 << ADSC); // Start the conversion
  return ADCH;
}

int main(void) {
  uint8_t adc;

  initIO();
  initADC();

  while (1) {
    adc = getADC();

    if (adc <= 5) {
      PIN_TOG;
      _delay_ms(50);
    } else if (adc >= 125 && adc <= 129) {
      PIN_TOG;
      _delay_ms(100);
    } else if (adc >= 250) {
      PIN_TOG;
      _delay_ms(200);
    } else
      PORTB &= ~(1 << LED); // Set 0
  }
}

/*
# Comments:
===================================
## What does the program do?
It reads an analog signal with further digital processing. Turn POT left or
right to reach threshold values that will trigger the LED.

Circuit connections:
PB0 [OC0A] - LED - 2,2k - GND
PB1 [OC0B] - *
PB2 - *
PB3 - POT VCC - 10k - GND
PB4 - *
PB5 - Reset (input)

## Notes:
Reading ADC value
---------------------------------------

ADLAR = 0, Right Adjustement, page 93
ADCH                        | ADCL
H  .  .  .  .  .  .    .    |    .    .    .    .    .    .    . L  .
----------------------------------------------------------------------
|  |  |  |  |  |  | b9 | b8 | b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 |


ADLAR = 1, Left Adjustement 8-bit precision
ADCH                                    | ADCL
H    .    .    .    .    .    .    . L  | H  .    .  .  .  .  .  .  L
----------------------------------------------------------------------
| b9 | b8 | b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 |  |  |  |  |  |  |


How not to be confused here, if we will read just only b9-b2 without b1-b0? The
answer is: The max value of ADCH when left adjusted will be 255 (0xff). When
right adjusted, the max value will be 3 (0x03). However, if need 10-bit
resolution result from the register should be shifted and added with the OR
operation.

ADCL
-----------------------------
| b1 | b0 |  |  |  |  |  |  |


ADCH
-----------------------------------------
| b9 | b8 | b7 | b6 | b5 | b4 | b3 | b2 |


Shifting ADCL to the right 6 places using this: ADCL >> 6
-----------------------------
|  |  |  |  |  |  | b1 | b0 |


result10bit |= ADCL >> 6 then would look like this
-----------------------------
|  |  |  |  |  |  |  |  |  |  |  |  |  |  | b1 | b0 |


result10bit |= ADCH << 2 | ADCL >> 6
-----------------------------
|  |  |  |  |  |  | b9 | b8 | b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 |


# Conclusion
===================================
Run this code and find what behavior is expected and what we got.
*/