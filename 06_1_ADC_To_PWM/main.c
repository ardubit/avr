/*
 * ADC to PWM output
 * Created: 21.06.2023
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
#define PWM0A_PIN PB0 // LED
#define PWM0B_PIN PB1
#define ANALOGPOT PB3

void initIO() {
  // DDRB '0' - input, '1' - output
  // Pullups resistors to 'VCC' or output logic states of pins
  // Input '1' - pullup enable, '0' - pullup disable

  // Reset pin as input
  DDRB &= ~(1 << RESET_PIN);
  PORTB |= (1 << RESET_PIN);

  // Set pin as output
  DDRB |= (1 << PWM0A_PIN);

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

void initPWM() {
  // Timer Setup
  // Start timer T0 with prescaler, page 73
  // Fast PWM freq: Fpwm = Fclock/ N * 256
  TCCR0B |= (1 << CS00); // No prescaler

  // Enable Fast PWM
  TCCR0A |= (1 << WGM01) | (1 << WGM00);

  // Compare Output Mode (page 69, 70)
  TCCR0A |= (1 << COM0A1) | (1 << COM0A0); // Inverting mode for full turn OFF

  /*
  In fast pwm mode, the duty cycle is: (OCRx + 1) / (TOP + 1) * 100%, thus you
  can never get to 0% in noninverting mode and never to 100% in inverting mode.
  */
}

uint8_t getADC() {
  ADCSRA |= (1 << ADSC); // Start the conversion
  return ADCH;
}

int main(void) {
  uint8_t adc;

  initIO();
  initADC();
  initPWM();

  while (1) {
    adc = getADC();
    OCR0A = adc;
  }
}

/*
# Comments:
===================================
## What does the program do?
The program reads an analog signal and uses its digital value to feed the OCRA register, which is responsible for generating a PWM waveform. Turn POT to set the brightness of the LED.

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
