#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

void Timer1_asTimer()
{
	/* TCNT1 starts from 0X0000 goes upto 0XFFFF and restarts */
	/* No possible use case as it just goes upto 0xFFFF and restarts */
	// MOde of operation to Normal Mode -- WGM1[3:0] === 0000
	/* WGM1[3](bit4) from TCCR1B, WGM1[2](bit3) from TCCR1B,
	 WGM1[1](bit1)  from TCC1RA, WGM1[0](bit0)  from TCCR1A*/
	TCCR1A = TCCR1A & ~(1<<WGM10);
	TCCR1A = TCCR1A & ~(1<<WGM11);
	TCCR1B = TCCR1B & ~(1<<WGM12);
	TCCR1B = TCCR1B & ~(1<<WGM13);	
	/* What to do when timer reaches the MAX(0xFFFF) value */
	// toggle OC1A on each time when reaches the MAX(0xFFFF)
	// which is reflected in PB1
	// Output OC1A to toglle when reaches MAX -- COM1A[1:0] === 01
	// COM1A[1](bit7) from TCCR1A, COM1A[0](bit6) from TCCR1A
	TCCR1A = TCCR1A & ~(1<<COM1A1);
	TCCR1A = TCCR1A | (1<<COM1A0);
	// toggle OC1B on each time when reaches the MAX(0xFFFF)
	// which is reflected in PB2
	// Output OC1B to toglle when reaches MAX -- COM1B[:0] === 01
	// COM1B[1](bit5) from TCCR1A, COM1B[0](bit4) from TCCR1A
	TCCR1A = TCCR1A & ~(1<<COM1B1);
	TCCR1A = TCCR1A | (1<<COM1B0);
	//Enable Interrupt of OVERFLOW flag so that interrupt can be generated
	TIMSK1 = TIMSK1 | (1<<TOV1);
	// start timer by setting the clock prescalar
	// SAME AS from I/O clock
	// same-- CS1[2:0] === 001
	// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
	TCCR1B = TCCR1B | (1<<CS10);
	TCCR1B = TCCR1B & ~(1<<CS11);
	TCCR1B = TCCR1B & ~(1<<CS12);
	// enabling global interrupt	
	sei();
	// SO ON TIME = max_count / (F_CPU / PRESCALAR)
	// ON TIME = 0xFFFF / (16000000/1) = 4.096ms
	// since symmetric as toggling OFF TIME = 4.096ms
	// hence, we get a square wave of fequency 1 / 8.192ms = 122.07Hz
		 
}
void Timer1_asCounter()
{
	// MOde of operation to Normal Mode -- WGM1[3:0] === 0000
	/* WGM1[3](bit4) from TCCR1B, WGM1[2](bit3) from TCCR1B, WGM1[1](bit1) 
	from TCC1RA, WGM1[0](bit0)  from TCCR1A	*/
	TCCR1A = TCCR1A & ~(1<<WGM10);
	TCCR1A = TCCR1A & ~(1<<WGM11);
	TCCR1B = TCCR1B & ~(1<<WGM12);
	TCCR1B = TCCR1B & ~(1<<WGM13);
	/* to count external event -we must connect source to T1 (PD5) */
	// THE CLK IS CLOCKED FROM external source
	// Falling edge of T1(PD5) -- CS1[2:0] === 110
	// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
	TCCR1B = TCCR1B & ~(1<<CS10);
	TCCR1B = TCCR1B | (1<<CS11);
	TCCR1B = TCCR1B | (1<<CS12);
}
void Timer1_asInputCapture()
{
	// MOde of operation to Normal Mode -- WGM1[3:0] === 0000
	/* WGM1[3](bit4) from TCCR1B, WGM1[2](bit3) from TCCR1B, WGM1[1](bit1) 
	 from TCC1RA, WGM1[0](bit0)  from TCCR1A	*/
	TCCR1A = TCCR1A & ~(1<<WGM10);
	TCCR1A = TCCR1A & ~(1<<WGM11);
	TCCR1B = TCCR1B & ~(1<<WGM12);
	TCCR1B = TCCR1B & ~(1<<WGM13);
	// Select the edge for Input Capture
	// ICES1(bit6) from TCCR1B
	// Capture on Rising edge, ICES1 === 1
	TCCR1B |= (1<<ICES1);
	//Enable Interrupt of Input Capture Interrupt Enable so that interrupt can be generated
	TIMSK1 = TIMSK1 | (1<<ICIE1);
	// start timer by setting the clock prescalar
	// SAME AS from I/O clock
	// same-- CS1[2:0] === 001
	// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
	TCCR1B = TCCR1B | (1<<CS10);
	TCCR1B = TCCR1B & ~(1<<CS11);
	TCCR1B = TCCR1B & ~(1<<CS12);
	// enabling global interrupt
	sei();
}

void Timer1_asDelay()
{
	/* TCNT1 starts from 0X0000 goes upto 0XFFFF and restarts */
	/* No possible use case as it just goes upto 0xFFFF and restarts */
	// MOde of operation to Normal Mode -- WGM1[3:0] === 0000
	/* WGM1[3](bit4) from TCCR1B, WGM1[2](bit3) from TCCR1B, WGM1[1](bit1)  
	from TCC1RA, WGM1[0](bit0)  from TCCR1A	*/
	TCCR1A = TCCR1A & ~(1<<WGM10);
	TCCR1A = TCCR1A & ~(1<<WGM11);
	TCCR1B = TCCR1B & ~(1<<WGM12);
	TCCR1B = TCCR1B & ~(1<<WGM13);	
	/* What to do when timer reaches the MAX(0xFFFF) value */
	// nothing should be done on OC1A for delay
	// nothing  -- COM1A[1:0] === 00
	// COM1A[1](bit7) from TCCR1A, COM1A[0](bit6) from TCCR1A
	TCCR1A = TCCR1A & ~(1<<COM1A1);
	TCCR1A = TCCR1A & ~(1<<COM1A0);		
	/* The delay possible = 0xffff / (F_CPU/prescalar) */
	// lowest delay = 0xffff / (16000000 / 1) = 4.096ms
	// when prescalar == 8 --> delay = 0xffff / (16000000 / 8) = 32.768ms
	// when prescalar == 64 --> delay = 0xffff / (16000000 / 64) = 262.144ms
	// when prescalar == 256 --> delay = 0xffff / (16000000 / 256) = 1.048576s
	// highest delay possible = 0xffff / (16000000 / 1024) = 4.194304s	
	// start timer by setting the clock prescalar
	// divede by 64 from I/O clock
	// divede by 64-- CS1[2:0] === 101
	// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
	TCCR1B = TCCR1B | (1<<CS10);
	TCCR1B = TCCR1B | (1<<CS11);
	TCCR1B = TCCR1B & ~(1<<CS12);
	// actual delaying - wait until delay happens
	while((TIFR1 & 0x01) == 0x00); // checking overflow flag when overflow happns
	// clearing the overflag so that we can further utilize
	TIFR1 = TIFR1 | 0x01;
}
volatile uint16_t capVal=0;
int main(void)
{	
    DDRB = DDRB | (1<<1) | (1<<2);
	DDRD = DDRD & ~(1<<5);
	DDRC |= (1<<0) | (1<<1);
	PORTC &= ~(1<<0);
	// Timer1_asTimer();
    // Timer1_asCounter();
	Timer1_asInputCapture();
    while(1)
    {
		// PORTC &= ~(1<<0);
		// Timer1_asDelay();
		// PORTC |= (1<<0);
		// Timer1_asDelay();
    }
}

ISR(TIMER1_OVF_vect)
{
    // toggle PC1 when overflows
	PINC |= (1<<1);
}

ISR(TIMER1_CAPT_vect)
{
	if((TIFR1 & (1<<ICF1)) != 0)
	{
		capVal = ICR1L;
		capVal = (ICR1H<<8) | (capVal & 0xFF);
		// see datamemory
	}
}