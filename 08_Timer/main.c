/*
 * Timer.c
 *
 * Created: 24.05.2015 16:55:36
 *  Author: LbookA
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

// ���������� �� ������������ �������� �������� �������
ISR (TIM0_OVF_vect) {
	LED2_TOG;
}

// ���������� �� ���������� �������� � ��������� ���������� OCR0A
//ISR (TIM0_COMPA_vect) {
	//asm("nop"); 
//}

// ���������� �� ���������� OCR0B
//ISR (TIM0_COMPB_vect) {
	//asm("nop"); 
//}

void setup () {
	// **** Port Setup **** //
	DDRB =  0b00000101;
	PORTB = 0b00111010;

	// **** Timer Setup **** //
	// �������� ������ (��� ��������)
	TCCR0B |= (1<<CS00);
	
	// ��������� ���������� �� ������� ��� ������������
	TIMSK0 |= (1<<TOIE0);	
	
	// ��������� ���������� �� ������� ��� ������������ ��������� ���������
	//TIMSK0 |= (0<<OCIE0A) | (0<<OCIE0B);	
	
	// ������� �������� � ������� ����������
	// OCR0A = 100;
	
	// ������� �������� � �������� ����������	
	// OCR0B = 200;	
	
	// ��������� ����������
	ACSR |= (1 << ACD);
	
	// �������� ����� �� ������������ �������������
	TIFR0 = 0;
	
	
}

int main(void)
{
	// ������ ���������� ����������
	cli ();
	
	setup ();
	
	// ���������� ���������� ����������
	sei ();
    while(1)
    {
       asm volatile ("nop"); 
    }
}