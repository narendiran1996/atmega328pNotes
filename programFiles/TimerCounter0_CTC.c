#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

void Timer0_asTimer()
{
	/* TCNT0 starts from 0X00 goes upto OCR0A and restarts */	
	// MOde of operation to CTC Mode -- WGM0[2:0] === 010
	// WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
	TCCR0A = TCCR0A & ~(1<<0);
	TCCR0A = TCCR0A | (1<<1);
	TCCR0B = TCCR0B & ~(1<<3);	
	/* What to do when timer reaches the OCR0A */
	// toggle OC0A on each time when reaches the OCR0A
	// which is reflected in PD6
	// Output OC0A to toglle when reaches MAX -- COM0A[1:0] === 01
	// COM0A[1](bit7) from TCCR0A, COM0A[0](bit6) from TCCR0A
	TCCR0A = TCCR0A & ~(1<<7);
	TCCR0A = TCCR0A | (1<<6);
	// Output OC0B to toglle when reaches MAX -- COM0B[1:0] === 01
	// COM0B[1](bit7) from TCCR0A, COM0B[0](bit6) from TCCR0A
	TCCR0A = TCCR0A & ~(1<<5);
	TCCR0A = TCCR0A | (1<<4);		
	// Enable Interrupt when counter matches OCR0A Rgister
	//  OCIE0A bit is enabled
	TIMSK0 = TIMSK0 | (1<<1);	
	// setting the value till the counter should reach in OCR0A
	// for toggling of OC0A pin
	OCR0A = 0x32;
	// start timer by setting the clock prescalar
	// DIVIDE BY 8 from I/O clock
	// DIVIDE BY 8-- CS0[2:0] === 010
	// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
	TCCR0B = TCCR0B | (1<<1);
	TCCR0B = TCCR0B & (~(1<<0) & ~(1<<2));	
	// enabling global interrupt
	sei();
	// SO ON TIME = (1 + OCR0A) / (F_CPU / PRESCALAR)
	// ON TIME = 0X32 / (16000000/8) = 25.5us
	// since symmetric as toggling OFF TIME = 25.5us
	// hence, we get a square wave of fequency 1 / 50us = 20kHz
}
void Timer0_asCounter()
{
	/* TCNT0 starts from 0X00 goes upto OCR0A and restarts */
	// MOde of operation to CTC Mode -- WGM0[2:0] === 010
	// WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
	TCCR0A = TCCR0A & ~(1<<0);
	TCCR0A = TCCR0A | (1<<1);
	TCCR0B = TCCR0B & ~(1<<3);		
	// Disbale Interrupt when counter matches OCR0A Rgister
	//  OCIE0A bit is disabled
	TIMSK0 = TIMSK0 | (1<<1);	
	//we count till OCR0A register value and reset and continue 
	OCR0A = 0xA;	
	/* to count external event -we must connect source to T0 (PD4) */
	// THE CLK IS CLOCKED FROM external source
	// Falling edge of T0(PD4) -- CS0[2:0] === 110
	// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
	TCCR0B = TCCR0B | (1<<2);
	TCCR0B = TCCR0B | (1<<1);
	TCCR0B = TCCR0B & ~(1<<0);	
	//enable global interrupt
	sei();
}
void Timer0_asDelayIn_ms(uint32_t delayInMs)
{
	// minimum delay being 4us -- choose like that
	// PRESCALAR OF 1 -- 3us - 16us -- usage 3us - 16us -- factor=0 -- CS0[2:0]=1
	// PRESCALAR OF 8 -- 3us - 128us -- usage 17us - 128us -- factor=3 -- CS0[2:0]=2
	// PRESCALAR OF 64 -- 4us - 1.024ms -- usage 129us - 1024us -- factor=6 -- CS0[2:0]=3
	// PRESCALAR OF 256 -- 16us - 4.096ms -- usage 1025us - 4096us -- factor=8 -- CS0[2:0]=4
	// MOde of operation to ctc Mode -- WGM0[2:0] === 010
	// WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
	TCCR0A = TCCR0A & ~(1<<0);
	TCCR0A = TCCR0A | (1<<1);
	TCCR0B = TCCR0B & ~(1<<3);
	while(delayInMs--)
	{
		// for 1ms delay
		OCR0A = 249;
		// start timer by setting the clock prescalar
		//  dived by 64 from I/O clock
		//  CS0[2:0] === 011
		// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
		TCCR0B = TCCR0B | (1<<0);
		TCCR0B = TCCR0B | (1<<1);
		TCCR0B = TCCR0B & ~(1<<2);
		// actual delaying - wait until delay happens
		// checking OCF0A (compare match flag A) flag when match happns
		while((TIFR0 & 0x02) == 0x00);
		// clearing the compare match flag so that we can further utilize
		TIFR0 = TIFR0 | 0x02;
	}
}
int main(void)
{	
	// making the PD5 and PD6 as output
	DDRD = DDRD | (1<<6) | (1<<5);
	DDRD = DDRD & ~(1<<4);
	DDRB |= (1<<0);
	DDRC |= (1<<0) | (1<<1);
	PORTC &= ~(1<<0);
	// Timer0_asTimer();
	// Timer0_asCounter();
	while(1)
	{
		PORTC &= ~(1<<0);
		Timer0_asDelayIn_ms(100);
		PORTC |= (1<<0);
		Timer0_asDelayIn_ms(100);
	}
}

ISR(TIMER0_COMPA_vect)
{
	// toggle PC1 when matches
	PINC |= (1<<1);
}