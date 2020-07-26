#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

void Timer1_asTimer()
{
	// MOde of operation to Normal Mode -- WGM1[3:0] === 0100(TOP = OCR1A) or 1100(TOP = ICR1)
	// WGM1[3](bit4) from TCCR1B, WGM1[2](bit3) from TCCR1B, WGM1[1](bit1)  from TCC1RA, WGM1[0](bit0)  from TCCR1A	
	// we take TOP to be OCR1A for custom frequency
	TCCR1A = TCCR1A & ~(1<<WGM10);
	TCCR1A = TCCR1A & ~(1<<WGM11);
	TCCR1B = TCCR1B | (1<<WGM12);
	TCCR1B = TCCR1B & ~(1<<WGM13);
	
	/* What to do when timer reaches the OCR1A value */
	// toggle OC1A on each time when reaches the OCR1A
	// which is reflected in PB1
	// Output OC1A to toglle when reaches OCR1A -- COM1A[1:0] === 01
	// COM1A[1](bit7) from TCCR1A, COM1A[0](bit6) from TCCR1A	
	TCCR1A = TCCR1A | (1<<COM1A0);
	TCCR1A = TCCR1A & ~(1<<COM1A1);	

	// toggle OC1B on each time when reaches the OCR1A
	// which is reflected in PB2
	// Output OC1B to toglle when reaches OCR1A -- COM1B[1:0] === 01
	// COM1B[1](bi57) from TCCR1A, COM1B[0](bit64) from TCCR1A	
	TCCR1A = TCCR1A | (1<<COM1B0);
	TCCR1A = TCCR1A & ~(1<<COM1B1);	
	
	// Enable Interrupt when counter matches OCR1A Rgister
	//  OCIE1A  bit is enabled
	TIMSK1 = TIMSK1 | (1<<OCIE1A);
	
	// setting the value till the counter should reach in OCR1A
	// for toggling of OC1A pin
	OCR1A = 0x4861;
		
	// start timer by setting the clock prescalar
	// SAME AS from I/O clock
	// same-- CS1[2:0] === 001
	// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
	TCCR1B = TCCR1B | (1<<CS10);
	TCCR1B = TCCR1B & ~(1<<CS11);
	TCCR1B = TCCR1B & ~(1<<CS12);
	
	// enabling global interrupt
	
	sei();
	// SO ON TIME = (1 + OCR1A) / (F_CPU / PRESCALAR)
	// ON TIME = 0x4861 / (16000000/1) = 1.15ms
	// since symmetric as toggling OFF TIME = 1.15ms
	// hence, we get a square wave of fequency 1 / 2.31ms = 431Hz	
}
void Timer1_asCounter()
{
	// MOde of operation to Normal Mode -- WGM1[3:0] === 0100(TOP = OCR1A) or 1100(TOP = ICR1)
	// WGM1[3](bit4) from TCCR1B, WGM1[2](bit3) from TCCR1B, WGM1[1](bit1)  from TCC1RA, WGM1[0](bit0)  from TCCR1A	
	TCCR1A = TCCR1A & ~(1<<WGM10);
	TCCR1A = TCCR1A & ~(1<<WGM11);
	TCCR1B = TCCR1B | (1<<WGM12);
	TCCR1B = TCCR1B & ~(1<<WGM13);
	
	/* What to do when timer reaches the OCR1A value */
	// toggle OC1A on each time when reaches the OCR1A
	// which is reflected in PB1
	// Output OC1A to toglle when reaches OCR1A -- COM1A[1:0] === 01
	// COM1A[1](bit7) from TCCR1A, COM1A[0](bit6) from TCCR1A
	TCCR1A = TCCR1A | (1<<COM1A0);
	TCCR1A = TCCR1A & ~(1<<COM1A1);
	
	
	//we count till OCR1A register value and toggle
	// lets' count 10 pulses
	OCR1A = 0x000a;
	
	/* to count external event -we must connect source to T1 (PD5) */
	// THE CLK IS CLOCKED FROM external source
	// Falling edge of T1(PD5) -- CS1[2:0] === 110
	// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
	TCCR1B = TCCR1B & ~(1<<CS10);
	TCCR1B = TCCR1B | (1<<CS11);
	TCCR1B = TCCR1B | (1<<CS12);
	
	
	// since for every rising edge the count increase
	// so to reach 10 count, it would take 0xa / (frequency of input at T1 pin or PD5)
	// we wave used 5kHz so it would take ==> 2ms to toggle as we have made OC1A toggle when overflows (by setting COMA[1:0])
	// also we canuse TCNT1 as edge counter
}
void Timer1_asDelayIn_us(uint32_t delay_in_us)
{
	// minimum delay being 4us -- choose like that - because, of the the delay for execution, - we get us if we use toggling of pins OC1A or OC1B
	// use PRESCALAR OF 1 -- 4us - 4.096ms -- usage 4us - 4ms -- factor=0 -- CS1[2:0]=1
	// use PRESCALAR OF 8 -- 4us - 32.768ms -- usage 5ms - 32ms -- factor=3 -- CS1[2:0]=2
	// use PRESCALAR OF 64 -- 4us - 262.144ms -- usage 33ms - 260ms -- factor=6 -- CS0[2:0]=3
	// use PRESCALAR OF 256 -- 16us - 1.048s -- usage 261ms - 1.048s -- factor=8 -- CS0[2:0]=4
	
	
	/* TCNT1 starts from 0X0000 goes upto OCR1A or ICR1 and restarts */	
	// MOde of operation to Normal Mode -- WGM1[3:0] === 0100(TOP = OCR1A) or 1100(TOP = ICR1)
	// WGM1[3](bit4) from TCCR1B, WGM1[2](bit3) from TCCR1B, WGM1[1](bit1)  from TCC1RA, WGM1[0](bit0)  from TCCR1A	
	// we take TOP to be OCR1A for custom frequency
	TCCR1A = TCCR1A & ~(1<<WGM10);
	TCCR1A = TCCR1A & ~(1<<WGM11);
	TCCR1B = TCCR1B | (1<<WGM12);
	TCCR1B = TCCR1B & ~(1<<WGM13);
		
	/* What to do when timer reaches the MAX(0xFFFF) value */
	// nothing should be done on OC1A for delay
	// nothing  -- COM1A[1:0] === 00
	// COM1A[1](bit7) from TCCR1A, COM1A[0](bit6) from TCCR1A
	TCCR1A = TCCR1A & ~(1<<COM1A1);
	TCCR1A = TCCR1A & ~(1<<COM1A0);
	

	
	if(delay_in_us <=3)
	{
		// if delay_in_us <= 3us -- so we stop clock
		
		OCR1A = 0;
		// stop clcok
		// stop clcok-- CS1[2:0] === 000
		// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
		TCCR1B = TCCR1B & ~(1<<CS10);
		TCCR1B = TCCR1B & ~(1<<CS11);
		TCCR1B = TCCR1B & ~(1<<CS12);
	}
	else if((3 < delay_in_us)  && (delay_in_us <= 4000))
	{
		OCR1A = ((delay_in_us * 16) >> 0) - 1;
		// start timer by setting the clock prescalar
		// SAME AS from I/O clock
		// same-- CS1[2:0] === 001
		// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
		TCCR1B = TCCR1B | (1<<CS10);
		TCCR1B = TCCR1B & ~(1<<CS11);
		TCCR1B = TCCR1B & ~(1<<CS12);
	}
	else if((4000 < delay_in_us)  && (delay_in_us <= 32000))
	{
		OCR1A = ((delay_in_us * 16) >> 3) - 1;
		// start timer by setting the clock prescalar
		// divide by 8 from I/O clock
		// divide by 8 CS1[2:0] === 010
		// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
		TCCR1B = TCCR1B & ~(1<<CS10);
		TCCR1B = TCCR1B | (1<<CS11);
		TCCR1B = TCCR1B & ~(1<<CS12);
	}
	else if((32000 < delay_in_us)  && (delay_in_us <= 260000))
	{
		OCR1A = ((delay_in_us * 16) >> 6) - 1;
		// start timer by setting the clock prescalar
		// divide by 64 from I/O clock
		// divide by 64 CS1[2:0] === 011
		// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
		TCCR1B = TCCR1B | (1<<CS10);
		TCCR1B = TCCR1B | (1<<CS11);
		TCCR1B = TCCR1B & ~(1<<CS12);
	}
	else if((260000 < delay_in_us)  && (delay_in_us <= 1000000))
	{
		OCR1A = ((delay_in_us * 16) >> 8) - 1;
		// start timer by setting the clock prescalar
		// divide by 256 from I/O clock
		// divide by 256 CS1[2:0] === 100
		// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
		TCCR1B = TCCR1B & ~(1<<CS10);
		TCCR1B = TCCR1B & ~(1<<CS11);
		TCCR1B = TCCR1B | (1<<CS12);
	}
	else if(delay_in_us > 1000000)
	{
		Timer1_asDelayIn_us(delay_in_us - 1000000);
		OCR1A = ((1000000 * 16) >> 8) - 1;
		// start timer by setting the clock prescalar
		// divide by 256 from I/O clock
		//divide by 256 CS1[2:0] === 100
		// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
		TCCR1B = TCCR1B & ~(1<<CS10);
		TCCR1B = TCCR1B & ~(1<<CS11);
		TCCR1B = TCCR1B | (1<<CS12);
	}
	
	// actual delaying - wait until delay happens
	while((TIFR1 & 0x02) == 0x00); // checking OCF1A (compare match flag A) flag when match happns
	// clearing the compare match flag so that we can further utilize
	TIFR1 = TIFR1 | 0x02;

}
int main(void)
{	
    DDRB = DDRB | (1<<1) | (1<<2);
	DDRD = DDRD & ~(1<<5);
	DDRC |= (1<<0);
	PORTC &= ~(1<<0);

	// Timer1_asTimer();
    // Timer1_asCounter();
    while(1)
    {
		PINC |= (1<<0);
		Timer1_asDelayIn_us(400);
    }
}

ISR(TIMER1_COMPA_vect)
{
    // do the thing when overflows.
	
}