/*
 * Software_Timers_Test.c
 *
 * Created: 04.09.2015 15:03:46
 *  Author: LbookA
 */ 


#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

uint8_t flags;
volatile uint8_t ISR_flags;

volatile uint16_t timer_1_counter_blink;			// Counting time for a LED blinking
volatile uint16_t timer_2_counter_long_press;	// Long press handling timer
volatile uint16_t timer_3_counter_double_click;	// Double click handling timer

uint8_t click_counter;

#define SET_BIT(p,m)	((p) |=(1<<(m)))
#define CLR_BIT(p,m)	((p) &=~(1<<(m)))
#define CHK_BIT(p,m)	((p) & (1<<(m)))
#define TOG_BIT(p,m)	((p) ^=(1<<(m)))

// Bits in the ISR_flags
#define TIMER_1_BLINK 			0		// Bit 0
#define TIMER_2_LONG_PRESS  	1		// Bit 1
#define TIMER_3_DOUBLE_CLICK  	2		// Bit 1

// Bits in the general variable flags
#define BUTTON_LATCH 		0		// Bit 0
#define DRIVE				1		// Bit 1
#define DRIVE_LATCH			2		// Bit 2
#define ONECKICK_LATCH		3		// Bit 3
#define BOOST				4		// Bit 4
#define MODE_LATCH			5		// Bit 5

// Click - 160 ms

#define BLINK_LED_PERIOD	800
#define LONG_PRESS_TIME		650		// ms
#define DOUBLE_CLICK_TIME	450		// ms
#define DEBOUNCE			45
#define DEBOUNCE_FAST		30

#define LED_GREEN_B_TOG		PORTB ^= (1<<PB2)
#define LED_GREEN_B_ON		PORTB |= (1<<PB2)
#define LED_GREEN_B_OFF		PORTB &=~(1<<PB2)
//
//#define LED_RED_D_TOG		PORTB ^= (1<<PB3)
//#define LED_RED_D_ON		PORTB |= (1<<PB3)
//#define LED_RED_D_OFF		PORTB &=~(1<<PB3)

#define RELAY_TOG			PORTB ^= (1<<PB0)
#define RELAY_ON			PORTB |= (1<<PB0)
#define RELAY_OFF			PORTB &=~(1<<PB0)

#define RELAY_ON			PORTB |= (1<<PB0)
#define RELAY_OFF			PORTB &=~(1<<PB0)

#define MUTE_ON				PORTB |= (1<<PB4)
#define MUTE_OFF			PORTB &=~(1<<PB4)

#define DRIVE_LED_ON		PORTB |= (1<<PB3)
#define DRIVE_LED_OFF		PORTB &=~(1<<PB3)

#define BOOST_ON			PORTB |= (1<<PB2)
#define BOOST_OFF			PORTB &=~(1<<PB2)

ISR (TIM0_COMPA_vect) {
	
	// Blinking LED handling timer
	timer_1_counter_blink++;
	if (timer_1_counter_blink >= BLINK_LED_PERIOD)
	{
		timer_1_counter_blink=0;
		SET_BIT(ISR_flags,TIMER_1_BLINK);
	}
	
	// Long press handling timer "One Click"
	timer_2_counter_long_press++;
	if (timer_2_counter_long_press >= LONG_PRESS_TIME)
	{
		timer_2_counter_long_press=0;
		SET_BIT(ISR_flags,TIMER_2_LONG_PRESS);
	}
	
	// Double click handling timer
	timer_3_counter_double_click++;
	if (timer_3_counter_double_click >= DOUBLE_CLICK_TIME)
	{
		timer_3_counter_double_click=0;
		SET_BIT(ISR_flags,TIMER_3_DOUBLE_CLICK);
	}
}

void MCU_Setup() {
	
	// PB0 - Relay
	// PB1 - Button
	// PB2 - LED Boost
	// PB3 - LED Drive
	// PB4 - Mute MOSFETs
	// PB5 - Reset (Pull-up 10k resistor to +Vcc)
	
	DDRB = (1<<PB0)|(0<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4)|(0<<PB5);
	PORTB = (1<<PB1)|(1<<PB5);
	
	// Analog comparator OFF
	ACSR |= (1<<ACD);
	
	// Start timer T0 with prescaler 8
	TCCR0B |= (1<<CS01);
	
	// Enable time T0 overflow interrupt
	TIMSK0 |= (1<<OCIE0A);
	
	// Enable CTC Mode.
	TCCR0A |= (1<<WGM01);
	
	// T0 will overflow each 1 ms
	OCR0A = 150;
	
	// Reset timer T0 flags
	TIFR0 = 0;
	
}

void Drive_ON() {
	
	MUTE_ON;
	_delay_ms(10);
	RELAY_ON;
	_delay_ms(25);
	MUTE_OFF;
	DRIVE_LED_ON;
	
}

void Drive_OFF() {
	
	MUTE_ON;
	_delay_ms(10);
	RELAY_OFF;
	_delay_ms(25);
	MUTE_OFF;
	DRIVE_LED_OFF;
	
}

void Clear_Timer_1_Blinking() {
	
	// Clear Blinking timer
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		timer_1_counter_blink=0;
		CLR_BIT(ISR_flags,TIMER_1_BLINK);
	}
	
}

void Clear_Timer_2_Long_Press() {
	
	// Clear "One Click" long press handling timer
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		timer_2_counter_long_press=0;
		CLR_BIT(ISR_flags,TIMER_2_LONG_PRESS);
	}
	
}

void Clear_Timer_3_Double_Click()
{
	// Clear double click handling timer
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		timer_3_counter_double_click=0;
		CLR_BIT(ISR_flags,TIMER_3_DOUBLE_CLICK);
	}
	
}

void Blinking() {
	
	if (CHK_BIT(ISR_flags,TIMER_1_BLINK))
	{
		
		// Toggle LED.
		LED_GREEN_B_TOG;
		
		// Toggle LED lighthouse
		
		/*
		
		LED_GREEN_B_ON;
		_delay_ms(20);
		LED_GREEN_B_OFF;
		_delay_ms(20);
		
		LED_GREEN_B_ON;
		_delay_ms(80);
		LED_GREEN_B_OFF;
		
		*/
		
		Clear_Timer_1_Blinking();
		
		
		
		// Interrupt protection before accessing variable
		
		/* 
		
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			CLR_BIT(ISR_flags,TIMER_1_BLINK);
		}
		
		*/
		
	}
	
}

void Flags_Handler() {
	
	// **********************************************
	// Bypass
	// **********************************************
	
	if ( CHK_BIT(flags,DRIVE) && (!CHK_BIT(flags,DRIVE_LATCH)) )
	{
		SET_BIT(flags,DRIVE_LATCH);
		Drive_ON();
	}
	
	if ( !CHK_BIT(flags,DRIVE) && (CHK_BIT(flags,DRIVE_LATCH)) )
	{
		CLR_BIT(flags,DRIVE_LATCH);
		Drive_OFF();
	}
	
	// **********************************************
	// Boost
	// **********************************************
	
	//if ( CHK_BIT(flags,DRIVE) && CHK_BIT(flags,BOOST) )
	//{
		//BOOST_ON;
	//}
	//else if (!CHK_BIT(flags,BOOST))
	//{
		//BOOST_OFF;
	//}
	//
	// **********************************************
	// Boost indicator
	// **********************************************
	
	if ( !CHK_BIT(flags,DRIVE) && CHK_BIT(flags,BOOST) )
	{
		Blinking();
	}
	
}

void Long_Click_Handler() {
	
	if ( !(PINB & (1<<PB1)) && (!CHK_BIT(flags,ONECKICK_LATCH)) && (CHK_BIT(ISR_flags,TIMER_2_LONG_PRESS)) )
	{
		SET_BIT(flags,ONECKICK_LATCH);
		TOG_BIT(flags,DRIVE);
		
		while (!(PINB & (1<<PB1)))
		{
			// asm("nop");
			
			// **********************************************
			// Boost indicator
			// **********************************************
			
			if ( CHK_BIT(flags,DRIVE) && CHK_BIT(flags,BOOST) )
			{
				Blinking();
			}
				
		}
		
	}
	
	// Button release
	
	if ( (PINB & (1<<PB1)) && (CHK_BIT(flags,ONECKICK_LATCH)) )
	{
		CLR_BIT(flags,ONECKICK_LATCH);
		_delay_ms(DEBOUNCE_FAST);
	}
	
	// Clear "One Click" timer bit
	
	
	if ( PINB & (1<<PB1) ) {
		
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
		CLR_BIT(ISR_flags,TIMER_2_LONG_PRESS);
		}
	}
	
	
	
}

void Double_Click_Handler() {
	
	if ( !(PINB & (1<<PB1)) )
	{
		_delay_ms(DEBOUNCE_FAST);
		
		if ( !(PINB & (1<<PB1)) && (!CHK_BIT(flags,BUTTON_LATCH)) )
		{
			SET_BIT(flags,BUTTON_LATCH);
			
			// Clear "One Click" long press handling timer
			
			Clear_Timer_2_Long_Press();
			
			// **********************************************
			// Reset clicks counter if reached max
			// **********************************************
			
			if (click_counter == 2)
			{
				click_counter=0;
			}
			
			// **********************************************
			// Lets start count clicks
			// **********************************************
			
			click_counter++;
			
			// **********************************************
			// If it's first click toggle a Drive bit
			// **********************************************
			
			if (click_counter == 1)
			{
				TOG_BIT(flags,DRIVE);
			}
			
			// **********************************************
			// Start count time for double click
			// **********************************************
			
			if (click_counter == 1)
			{
				Clear_Timer_3_Double_Click();
			}
			
		}
		
	}
	
	// Button release
	
	if ( (PINB & (1<<PB1)) && (CHK_BIT(flags,BUTTON_LATCH)) )
	{
		CLR_BIT(flags,BUTTON_LATCH);
		//_delay_ms(DEBOUNCE_FAST);
	}
	
	// **********************************************
	// Analyzer 
	// **********************************************
	
	if ( (click_counter == 1) && (CHK_BIT(ISR_flags,TIMER_3_DOUBLE_CLICK)) )
	{
		
		CLR_BIT(ISR_flags,TIMER_3_DOUBLE_CLICK);
		click_counter=0;
		
		CLR_BIT(flags,MODE_LATCH);
		
	}
	
	if ( (click_counter == 2) && (!CHK_BIT(ISR_flags,TIMER_3_DOUBLE_CLICK)) )
	{
		
		// Got two clicks in time, so it's a double click action
		TOG_BIT(flags,BOOST);
		
		// Turn back relay
		TOG_BIT(flags,DRIVE);
		
		CLR_BIT(ISR_flags,TIMER_3_DOUBLE_CLICK);
		click_counter=0;
		
	}
	
	// Keep double click timer bit clear
	
	if (click_counter == 0) 
	{
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
		CLR_BIT(ISR_flags,TIMER_3_DOUBLE_CLICK);
		}
	}
	
}

void Double_Click_Protection() {
	
	// Clear double click handling timer
	
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		timer_3_counter_double_click=0;
		CLR_BIT(ISR_flags,TIMER_3_DOUBLE_CLICK);
	}
	
}

void Button_Handler() {
	
	if ( !(PINB & (1<<PB1)) )
	{
		_delay_ms(DEBOUNCE);
		
		if ( !(PINB & (1<<PB1)) && (!CHK_BIT(flags,BUTTON_LATCH)) )
		{
			SET_BIT(flags,BUTTON_LATCH);
			TOG_BIT(flags,DRIVE);
			
			//Launch double click protection
			Double_Click_Protection();
			
			// Clear "One Click" long press handling timer
			Clear_Timer_2_Long_Press();
			
		}
		
	}
	
	// Set mode latch if protection time is lost
	
	if ( (CHK_BIT(flags,DRIVE)) && (CHK_BIT(ISR_flags,TIMER_3_DOUBLE_CLICK)) )
	{
		SET_BIT(flags,MODE_LATCH);
	}
	
	// Button release
	
	if ( (PINB & (1<<PB1)) && (CHK_BIT(flags,BUTTON_LATCH)) )
	{
		CLR_BIT(flags,BUTTON_LATCH);
	}
	
}

int main(void)
{
	
	MCU_Setup();
	sei();
	
	while(1)
	{
		
		Blinking();
		
		Button_Handler();
		Flags_Handler();
		Long_Click_Handler();
		Double_Click_Handler();
		
		// **********************************************
		// If we have "Drive" mode, lets call double click handler function
		// **********************************************
		
		 while ( (CHK_BIT(flags,MODE_LATCH)) )
		{
			Double_Click_Handler();
			//Flags_Handler();
			//Long_Click_Handler();
		}
		
	}
	
}
	
