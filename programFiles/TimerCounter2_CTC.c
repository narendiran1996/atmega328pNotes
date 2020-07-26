#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

void Timer2_asTimer()
{
	// MOde of operation to CTC Mode -- WGM2[2:0] === 010
	// WGM2[2](bit3) from TCCR2B, WGM2[1](bit1)  from TCCR2A, WGM2[0](bit0)  from TCCR2A
	TCCR2A = TCCR2A & ~(1<<0);
	TCCR2A = TCCR2A | (1<<1);
	TCCR2B = TCCR2B & ~(1<<3);

	/* What to do when timer reaches the OCR2A */
	// toggle OC2A on each time when reaches the OCR2A
	// which is reflected in PB3
	// Output OC2A to toglle when reaches MAX -- COM2A[1:0] === 01
	// COM2A[1](bit7) from TCCR2A, COM2A[0](bit6) from TCCR2A
	TCCR2A = TCCR2A & ~(1<<7);
	TCCR2A = TCCR2A | (1<<6);

	// Output OC2B to toglle when reaches MAX -- COM2B1:0] === 01
	// COM2B[1](bit7) from TCCR2A, COM2B[0](bit6) from TCCR2A
	TCCR2A = TCCR2A & ~(1<<5);
	TCCR2A = TCCR2A | (1<<4);

		
	// Enable Interrupt when counter matches OCR2A Rgister
	//  OCIE2A bit is enabled
	TIMSK2 = TIMSK2 | (1<<1);


	// setting the value till the counter should reach in OCR2A
	// for toggling of OC2A pin
	OCR2A = 0x32;

	// start timer by setting the clock prescalar
	// DIVIDE BY 8 from I/O clock
	// DIVIDE BY 8-- CS2[2:0] === 010
	// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
	TCCR2B = TCCR2B | (1<<1);
	TCCR2B = TCCR2B & (~(1<<0) & ~(1<<2));

	// enabling global interrupt
	sei();
	// SO ON TIME = (1 + OCR2A) / (F_CPU / PRESCALAR)
	// ON TIME = 0X32 / (16000000/8) = 25.5us
	// since symmetric as toggling OFF TIME = 25.5us
	// hence, we get a square wave of fequency 1 / 50us = 20kHz
}
void Timer2_asDelayIn_ms(uint32_t delayInMs)
{
	
	// minimum delay being 4us -- choose like that
	// use PRESCALAR OF 1 -- 3us - 16us -- usage 3us - 16us -- factor=0 -- CS2[2:0]=1
	// use PRESCALAR OF 8 -- 3us - 128us -- usage 17us - 128us -- factor=3 -- CS2[2:0]=2
	// use PRESCALAR OF 64 -- 4us - 1.024ms -- usage 129us - 1024us -- factor=6 -- CS2[2:0]=3
	// use PRESCALAR OF 256 -- 16us - 4.096ms -- usage 1025us - 4096us -- factor=8 -- CS2[2:0]=4
		
	// MOde of operation to ctc Mode -- WGM2[2:0] === 010
	// WGM2[2](bit3) from TCCR2B, WGM2[1](bit1)  from TCCR2A, WGM2[0](bit0)  from TCCR2A
	TCCR2A = TCCR2A & ~(1<<0);
	TCCR2A = TCCR2A | (1<<1);
	TCCR2B = TCCR2B & ~(1<<3);

	while(delayInMs--)
	{
		// for 1ms delay
		OCR2A = 249;
		// start timer by setting the clock prescalar
		//  dived by 64 from I/O clock
		//  CS2[2:0] === 011
		// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
		TCCR2B = TCCR2B | (1<<0);
		TCCR2B = TCCR2B | (1<<1);
		TCCR2B = TCCR2B & ~(1<<2);

		// actual delaying - wait until delay happens
		while((TIFR2 & 0x02) == 0x00); // checking OCF0A (compare match flag A) flag when match happns
		// clearing the compare match flag so that we can further utilize
		TIFR2 = TIFR2 | 0x02;
	}

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
		Timer2_asDelayIn_ms(10);
		PORTC |= (1<<0);
		Timer2_asDelayIn_ms(10);
    }
}

ISR(TIMER2_COMPA_vect)
{
    // do the thing when overflows.
}
