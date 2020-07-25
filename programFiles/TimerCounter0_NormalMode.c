#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

void Timer0_asTimer()
{
	
	// MOde of operation to Normal Mode -- WGM0[2:0] === 000
	// WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
	TCCR0A = TCCR0A & (~(1<<0) & ~(1<<1));
	TCCR0B = TCCR0B & ~(1<<3);
	
	/* What to do when timer reaches the MAX(0xFF) value */	
	// toggle OC0A and OC0B on each time when reaches the MAX(0xFF) 
	// which is reflected in PD6 and PD5
    
	// Output OC0A to toglle when reaches MAX -- COM0A[1:0] === 01
	// COM0A[1](bit7) from TCCR0A, COM0A[0](bit6) from TCCR0A
	TCCR0A = TCCR0A & ~(1<<7);
	TCCR0A = TCCR0A | (1<<6);

    // Output OC0B to toglle when reaches MAX -- COM0B1:0] === 01
	// COM0B[1](bit7) from TCCR0A, COM0B[0](bit6) from TCCR0A
	TCCR0A = TCCR0A & ~(1<<5);
	TCCR0A = TCCR0A | (1<<4);
	
	//Enable Interrupt of OVERFLOW flag so that interrupt can be generated
	TIMSK0 = TIMSK0 | (1<<0);	
	
	// start timer by setting the clock prescalar
	// DIVIDE BY 8 from I/O clock
	// DIVIDE BY 8-- CS0[2:0] === 010
	// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
	TCCR0B = TCCR0B | (1<<1);
	TCCR0B = TCCR0B & (~(1<<0) & ~(1<<2));
	
	// enabling global interrupt
	sei();
	// SO ON TIME = max_count / (F_CPU / PRESCALAR)
	// ON TIME = 0xFF / (16000000/8) = 128us
	// since symmetric as toggling OFF TIME = 128us
	// hence, we get a square wave of fequency 1 / 256us = 3.906kHz
		 
}
void Timer0_asCounter()
{
	// MOde of operation to Normal Mode -- WGM0[2:0] === 000
	// WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
	TCCR0A = TCCR0A & (~(1<<0) & ~(1<<1));
	TCCR0B = TCCR0B & ~(1<<3);
		
	/* to count external event -we must connect source to T0 (PD4) */
	// THE CLK IS CLOCKED FROM external source
	// Falling edge of T0(PD4) -- CS0[2:0] === 110
	// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
	TCCR0B = TCCR0B | (1<<2);
	TCCR0B = TCCR0B | (1<<1);
	TCCR0B = TCCR0B & ~(1<<0);	
}
void Timer0_asDelay()
{
	/* TCNT0 starts from 0X00 goes upto 0XFF and restarts */
	/* No possible use case as it just goes upto 0xFF and restarts */
	// MOde of operation to Normal Mode -- WGM0[2:0] === 000
	// WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
	TCCR0A = TCCR0A & (~(1<<0) & ~(1<<1));
	TCCR0B = TCCR0B & ~(1<<3);
	
	/* What to do when timer reaches the MAX(0xFF) value */
	// nothing should be done on OC0A for delay
	// nothing  -- COM0A[1:0] === 00
	// COM0A[1](bit7) from TCCR0A, COM0A[0](bit6) from TCCR0A
	TCCR0A = TCCR0A & ~(1<<7);
	TCCR0A = TCCR0A & ~(1<<6);
		
	/* The delay possible = 0xff / (F_CPU/prescalar) */
	// lowest delay = 0xff / (16000000 / 1) = 16us
	// when prescalar == 8 --> delay = 0xff / (16000000 / 8) = 128us
	// when prescalar == 64 --> delay = 0xff / (16000000 / 64) = 1.024ms
	// when prescalar == 256 --> delay = 0xff / (16000000 / 256) = 4.096ms
	// highest delay possible = 0xff / (16000000 / 1024) = 16.38ms
	
	// start timer by setting the clock prescalar
	// DIVIDE BY 8 use the same clock from I/O clock
	// DIVIDE BY 8-- CS0[2:0] === 010
	// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
	TCCR0B = TCCR0B & ~(1<<0);
	TCCR0B = TCCR0B | (1<<1);
	TCCR0B = TCCR0B & ~(1<<2);
	
	
	// actual delaying - wait until delay happens
	while((TIFR0 & 0x01) == 0x00); // checking overflow flag when overflow happns
	// clearing the overflag so that we can further utilize
	TIFR0 = TIFR0 | 0x01;
}
int main(void)
{	
    // making the PD5 and PD6 as output
    DDRD = DDRD | (1<<6) | (1<<5);
	DDRD = DDRD & ~(1<<4);
	DDRC |= (1<<0);
	PORTC &= ~(1<<0);

	// Timer0_asTimer();
    // Timer0_asCounter();
    while(1)
    {
		PORTC &= ~(1<<0);
		Timer0_asDelay();
		PORTC |= (1<<0);
		Timer0_asDelay();
    }
}

ISR(TIMER0_OVF_vect)
{
    // do the thing when overflows.
}