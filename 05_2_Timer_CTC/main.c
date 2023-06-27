/*
 * Generate PWM output in parallel with LED blink
 * Created: 30.05.2015 14:48:31
 * Author: Aleksey M.
 * MCU ATtiny13
 *
 */

#define F_CPU 1200000UL 
#include <avr/io.h>
#include <avr/interrupt.h>
#define LED0_ON PORTB = PORTB | (1<<PB0)
#define LED0_OFF PORTB = PORTB & ~(1<<PB0)
#define LED2_ON PORTB = PORTB | (1<<PB2)
#define LED2_OFF PORTB = PORTB & ~(1<<PB2)
#define LED2_TOG PORTB ^=(1<<PB2)
#define LED0_TOG PORTB ^=(1<<PB0)
#define PRESCALE_1024 (1<<CS00) | (1<<CS02)
#define PRESCALE_NO (1<<CS00)

volatile unsigned char a=0;

ISR (TIM0_OVF_vect) {
	a++;
	if (a==1)
	{
		LED2_TOG;
		a=0;
	}
}

// ���������� �� ���������� �������� � ��������� ���������� OCR0A
ISR (TIM0_COMPA_vect) {
	asm("nop"); 
}

// ���������� �� ���������� OCR0B
ISR (TIM0_COMPB_vect) {
	asm("nop"); 
}

void setup () {
	// **** Port B Setup **** //

	// PB3 - LED - 2, 2k - GND


	DDRB = 0b00000111;
	PORTB = 0b00111000;

	// **** Timer Setup **** //
	// ��������� ����������
	ACSR |= (1 << ACD); 
	
	// �������� ������ (� ��������� 1024)
	TCCR0B |= PRESCALE_NO;
	
	// ��������� ���������� �� �������	
	TIMSK0 |= (1<<TOIE0) | (1<<OCIE0A) | (1<<OCIE0B); 
	
	// ������� �������� � �������� ����������
	OCR0A = 199;
	
	// ������� �������� � �������� ����������
	OCR0B = 199;
	
	// �������� CTC ����� "����� ��� ���������"
	TCCR0A |= (1<<WGM01);
	
	// ����� OC0A ��������� � �������� ���������� OCR0A
	TCCR0A |= (1<<COM0A0); 
	
	// ����� OC0B ��������� � �������� ���������� OCR0B
	TCCR0A |= (1<<COM0B0); 
	
}

int main(void)
{
	setup ();
	sei ();
    while(1)
    {
        asm("nop"); 
    }
}