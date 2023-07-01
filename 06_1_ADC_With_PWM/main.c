/*
 * ADC with PWM output
 * Created: 21.06.2023
 * Author: Aleksey M.
 * MCU ATtiny13
 * Tested
 */

// MCU Frequency - 1,2 MHz. 9,6 MHz default clock with prescaling of 8. Check
// datasheet page 26.

#include <avr/io.h>
#include <avr/iotn13.h>
#include <util/delay.h>

#define F_CPU 1200000UL
#define DEBOUNCE_DELAY 20

#define LED_PIN PB3
#define RESET_PIN PB5
#define PWM0A_PIN PB0

void initIO() {
  // Define direction for port pins. "0" - input, "1" - output.
  // Define pullups resistors to "VCC" or output logic states of pins.
  // Input "1" - pullup enable, "0" - pullup disable.

  // Set (LED_PIN) as output
  DDRB |= (1 << LED_PIN);

  // Reset pin as input
  DDRB &= ~(1 << RESET_PIN);
  PORTB |= (1 << RESET_PIN);
}

void initADC() {
  // Set the ADC input to PB2/ADC1
  ADMUX |= (1 << MUX0);

  // Left adjust result
  ADMUX |= (1 << ADLAR);

  // Set the prescaler to clock/16 (1.2Mhz/16 = 75Khz)
  ADCSRA |= (1 << ADPS2);

  // ADC Enable
  ADCSRA |= (1 << ADEN);
}

void initPWM() {
  // Set pin as Output
  DDRB |= (1 << PWM0A_PIN);

  // Timer Setup
  // Start timer T0 with prescaler (page 73)
  // Fast PWM freq: Fpwm = Fclock/ N * 256
  TCCR0B |= (1 << CS00);

  // Enable Fast PWM
  TCCR0A |= (1 << WGM01) | (1 << WGM00);

  // Compare Output Mode (page 69, 70)
  TCCR0A |= (1 << COM0A1);
}

uint8_t getADC() {
  // Start the conversion
  ADCSRA |= (1 << ADSC);
  return ADCH;
}

void setPWM(uint8_t val) { OCR0A = val; }

int main(void) {
  uint8_t adc;

  initIO();
  initADC();
  initPWM();

  while (1) {
    adc = getADC();
    setPWM(adc);
  }
}