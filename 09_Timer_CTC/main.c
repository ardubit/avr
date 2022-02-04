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
#define PRESCALE_1024 (1<<CS00) | (1<<CS02)
#define PRESCALE_NO (1<<CS00)

volatile unsigned char a=0;

// Прерывание по переполнению счетного регистра таймера
ISR (TIM0_OVF_vect) {
	a++;
	if (a==1)
	{
		LED2_TOG;
		a=0;
	}
}

// Прерывание по совпадению счетчика с регистром совпадения OCR0A
ISR (TIM0_COMPA_vect) {
	asm("nop"); 
}

// Прерывание по совпадению OCR0B
ISR (TIM0_COMPB_vect) {
	asm("nop"); 
}

void setup () {
	// **** Port B Setup **** //
	DDRB =  0b00000111;
	PORTB = 0b00111000;

	// **** Timer Setup **** //
	// Отключаем компаратор
	ACSR |= (1 << ACD); 
	
	// Включаем таймер (с делителем 1024)
	TCCR0B |= PRESCALE_NO;
	
	// Разрешаем прерывания от таймера	
	TIMSK0 |= (1<<TOIE0) | (1<<OCIE0A) | (1<<OCIE0B); 
	
	// Заносим значение в регистры совпадения
	OCR0A = 199;
	
	// Заносим значение в регистры совпадения
	OCR0B = 199;
	
	// Включаем CTC режим "Сброс при совпадени"
	TCCR0A |= (1<<WGM01);
	
	// Вывод OC0A подключен к регистру совпадения OCR0A
	TCCR0A |= (1<<COM0A0); 
	
	// Вывод OC0B подключен к регистру совпадения OCR0B
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