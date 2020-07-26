#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

void Timer2_asTimer()
{
	// MOde of operation to Normal Mode -- WGM2[2:0] === 000
	// WGM2[2](bit3) from TCCR2B, WGM2[1](bit1)  from TCCR2A, WGM2[0](bit0)  from TCCR2A
	TCCR2A = TCCR2A & (~(1<<0) & ~(1<<1));
	TCCR2B = TCCR2B & ~(1<<3);

	/* What to do when timer reaches the MAX(0xFF) value */	
	// toggle OC2A and OC2B on each time when reaches the MAX(0xFF) 
	// which is reflected in PB3 and PD3

	// Output OC2A to toglle when reaches MAX -- COM2A[1:0] === 01
	// COM2A[1](bit7) from TCCR2A, COM2A[0](bit6) from TCCR2A
	TCCR2A = TCCR2A & ~(1<<7);
	TCCR2A = TCCR2A | (1<<6);

	// Output OC2B to toglle when reaches MAX -- COM2B1:0] === 01
	// COM2B[1](bit7) from TCCR2A, COM2B[0](bit6) from TCCR2A
	TCCR2A = TCCR2A & ~(1<<5);
	TCCR2A = TCCR2A | (1<<4);

	//Enable Interrupt of OVERFLOW flag so that interrupt can be generated
	TIMSK2 = TIMSK2 | (1<<0);	

	// start timer by setting the clock prescalar
	// DIVIDE BY 8 from I/O clock
	// DIVIDE BY 8-- CS2[2:0] === 010
	// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
	TCCR2B = TCCR2B | (1<<1);
	TCCR2B = TCCR2B & (~(1<<0) & ~(1<<2));

	// enabling global interrupt
	sei();
	// SO ON TIME = max_count / (F_CPU / PRESCALAR)
	// ON TIME = 0xFF / (16000000/8) = 128us
	// since symmetric as toggling OFF TIME = 128us
	// hence, we get a square wave of fequency 1 / 256us = 3.906kHz
}

void Timer2_asDelay()
{
	/* TCNT2 starts from 0X00 goes upto 0XFF and restarts */
	/* No possible use case as it just goes upto 0xFF and restarts */
	// MOde of operation to Normal Mode -- WGM2[2:0] === 000
	// WGM2[2](bit3) from TCCR2B, WGM2[1](bit1)  from TCCR2A, WGM2[0](bit0)  from TCCR2A
	TCCR2A = TCCR2A & (~(1<<0) & ~(1<<1));
	TCCR2B = TCCR2B & ~(1<<3);

	/* What to do when timer reaches the MAX(0xFF) value */
	// nothing should be done on OC2A for delay
	// nothing  -- COM2A[1:0] === 00
	// COM2A[1](bit7) from TCCR2A, COM2A[0](bit6) from TCCR2A
	TCCR2A = TCCR2A & ~(1<<7);
	TCCR2A = TCCR2A & ~(1<<6);
		
	/* The delay possible = 0xff / (F_CPU/prescalar) */
	// lowest delay = 0xff / (16000000 / 1) = 16us
	// when prescalar == 8 --> delay = 0xff / (16000000 / 8) = 128us
	// when prescalar == 64 --> delay = 0xff / (16000000 / 64) = 1.024ms
	// when prescalar == 256 --> delay = 0xff / (16000000 / 256) = 4.096ms
	// highest delay possible = 0xff / (16000000 / 1024) = 16.38ms

	// start timer by setting the clock prescalar
	// DIVIDE BY 8 use the same clock from I/O clock
	// DIVIDE BY 8-- CS2[2:0] === 010
	// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
	TCCR2B = TCCR2B & ~(1<<0);
	TCCR2B = TCCR2B | (1<<1);
	TCCR2B = TCCR2B & ~(1<<2);


	// actual delaying - wait until delay happens
	while((TIFR2 & 0x01) == 0x00); // checking overflow flag when overflow happns
	// clearing the overflag so that we can further utilize
	TIFR2 = TIFR2 | 0x01;
}
int main(void)
{	
    // making the PD5 and PD6 as output
    DDRD = DDRD | (1<<3);
    DDRB = DDRB | (1<<3);
	DDRC |= (1<<0);
	PORTC &= ~(1<<0);

	//Timer2_asTimer();

    while(1)
    {
    
		PORTC &= ~(1<<0);
		Timer2_asDelay();
		PORTC |= (1<<0);
		Timer2_asDelay();
		
    }
}

ISR(TIMER2_OVF_vect)
{
    // do the thing when overflows.
}
