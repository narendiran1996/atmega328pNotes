#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
void Timer1_NonInverting_TOP_at_MAX()
{
	/* TCNT1 starts from 0X0000 goes upto TOP and restarts from 0X00*/
	/* Mode of operation:
		WGM1[3:0] --> 0101 --	TOP--> 0X00FF
		WGM1[3:0] --> 0110 --	TOP--> 0x01FF
		WGM1[3:0] --> 0111 --	TOP--> 0x03FF
		WGM1[3:0] --> 1110 --	TOP--> ICR1
		WGM1[3:0] --> 1111 --	TOP--> OCR1A
	*/	
	// we take 0x03FF for fixed frequency and OCR1B for PWM on time(duty cycle)	
	// choose WGM1[3:0] --> 0111 for OCR1A as TOP for custom frequency
	TCCR1A = TCCR1A | (1<<WGM10);
	TCCR1A = TCCR1A | (1<<WGM11);
	TCCR1B = TCCR1B | (1<<WGM12);
	TCCR1B = TCCR1B & ~(1<<WGM13);    
    // here we set COM0A[1:0] as 10 for non-inverting
	// here we set COM0B[1:0] as 10 for non-inverting	
	// which is reflected in PD6
	// COM1A[1](bit7) from TCCR1A, COM1A[0](bit6) from TCCR1A
	TCCR1A = TCCR1A | (1<<COM1A1);
	TCCR1A = TCCR1A & ~(1<<COM1A0);	
	// which is reflected in PD65
	// COM1B[1](bit5) from TCCR1A, COM1B[0](bit4) from TCCR1A	
	TCCR1A = TCCR1A | (1<<COM1B1);
	TCCR1A = TCCR1A & ~(1<<COM1B0);
    // Enable Interrupt when TOV1 overflows TOP - here 0x03FF
	//  TOIE1 bit is enabled
	TIMSK1 = TIMSK1 | (1<<TOIE1);	
	/* we use OCF1A flag - which is set at every time TCN0 reaches OCR1A */
	TIMSK1 = TIMSK1 | (1<<OCIE1A);
	/* we use OCF1B flag - which is set at every time TCN0 reaches OCR1B */
	TIMSK1 = TIMSK1 | (1<<OCIE1B);
	// Next we set values for OCR1A and OCR2B
	/* Since, TCNT1 goes till max(0x3FF), we can choose OCR1A and 
	OCR1B to any value below max(0x03FF)*/
	OCR1A = 102; // for 10% duty clcle
	OCR1B = 767; // for 75% duty clcle
	// start timer by setting the clock prescalar
	// SAME AS from I/O clock
	// same-- CS1[2:0] === 001
	// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
	TCCR1B = TCCR1B | (1<<CS10);
	TCCR1B = TCCR1B & ~(1<<CS11);
	TCCR1B = TCCR1B & ~(1<<CS12);
	//enabled global interrupt
	sei();
}
void Timer1_Inverting_TOP_at_MAX()
{
	/* TCNT1 starts from 0X0000 goes upto TOP and restarts from 0X00*/
	/* Mode of operation:
	WGM1[3:0] --> 0101 --	TOP--> 0X00FF
	WGM1[3:0] --> 0110 --	TOP--> 0x01FF
	WGM1[3:0] --> 0111 --	TOP--> 0x03FF
	WGM1[3:0] --> 1110 --	TOP--> ICR1
	WGM1[3:0] --> 1111 --	TOP--> OCR1A	*/	
	// we take 0x03FF for fixed frequency and OCR1B for PWM on time(duty cycle)	
	// choose WGM1[3:0] --> 0111 for OCR1A as TOP for custom frequency
	TCCR1A = TCCR1A | (1<<WGM10);
	TCCR1A = TCCR1A | (1<<WGM11);
	TCCR1B = TCCR1B | (1<<WGM12);
	TCCR1B = TCCR1B & ~(1<<WGM13);
	// here we set COM0A[1:0] as 11 for inverting
	// here we set COM0B[1:0] as 11 for inverting
	// which is reflected in PD6
	// COM1A[1](bit7) from TCCR1A, COM1A[0](bit6) from TCCR1A
	TCCR1A = TCCR1A | (1<<COM1A1);
	TCCR1A = TCCR1A | (1<<COM1A0);
	// which is reflected in PD65
	// COM1B[1](bit5) from TCCR1A, COM1B[0](bit4) from TCCR1A	
	TCCR1A = TCCR1A | (1<<COM1B1);
	TCCR1A = TCCR1A | (1<<COM1B0);
	// Enable Interrupt when TOV1 overflows TOP - here 0x03FF
	//  TOIE1 bit is enabled
	TIMSK1 = TIMSK1 | (1<<TOIE1);
	/* we use OCF1A flag - which is set at every time TCN0 reaches OCR1A */
	TIMSK1 = TIMSK1 | (1<<OCIE1A);
	/* we use OCF1B flag - which is set at every time TCN0 reaches OCR1B */
	TIMSK1 = TIMSK1 | (1<<OCIE1B);		
	// Next we set values for OCR1A and OCR2B
	/* Since, TCNT1 goes till max(0x3FF), we can choose OCR1A and OCR1B to
	 any value below max(0x03FF)*/
	OCR1A = 102; // for 10% duty clcle
	OCR1B = 767; // for 75% duty clcle
	// start timer by setting the clock prescalar
	// SAME AS from I/O clock
	// same-- CS1[2:0] === 001
	// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
	TCCR1B = TCCR1B | (1<<CS10);
	TCCR1B = TCCR1B & ~(1<<CS11);
	TCCR1B = TCCR1B & ~(1<<CS12);
	//enabled global interrupt
	sei();
}
void Timer1_NonInverting_TOP_at_OCR1A()
{
	/* TCNT1 starts from 0X0000 goes upto TOP and restarts from 0X00*/
	/* Mode of operation:
		WGM1[3:0] --> 0101 --	TOP--> 0X00FF
		WGM1[3:0] --> 0110 --	TOP--> 0x01FF
		WGM1[3:0] --> 0111 --	TOP--> 0x03FF
		WGM1[3:0] --> 1110 --	TOP--> ICR1
		WGM1[3:0] --> 1111 --	TOP--> OCR1A	*/	
	// we take OCR1A for custom frequency and OCR1B for PWM on time(duty cycle)	
	// choose WGM1[3:0] --> 1111 for OCR1A as TOP for custom frequency
	TCCR1A = TCCR1A | (1<<WGM10);
	TCCR1A = TCCR1A | (1<<WGM11);
	TCCR1B = TCCR1B | (1<<WGM12);
	TCCR1B = TCCR1B | (1<<WGM13);
	// for non-inverting on  OC1B we use 10 for and COM1B[1:0]	
	// COM1B[1](bit5) from TCCR1A, COM1B[0](bit4) from TCCR1A
	TCCR1A = TCCR1A & ~(1<<COM1B0);
	TCCR1A = TCCR1A | (1<<COM1B1);
	// Next we set values for OCR1A and OCR1B
	// Since, TCNT1 goes till OCR1A, we can choose OCR1B to any value below OCR1A
	OCR1A = 0x7869; // for freqeuncy
	OCR1B = 0x1A20; // for pwm duty cylc
	// Enable interrupt when count reaches the overflow value
	TIMSK1 |= (1<<TOV1);
	// Enable interrupt when count reaches the OCR1B
	TIMSK1 |= (1<<OCF1B);	
	// start timer by setting the clock prescalar
	// SAME AS from I/O clock
	// same-- CS1[2:0] === 001
	// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
	TCCR1B = TCCR1B | (1<<CS10);
	TCCR1B = TCCR1B & ~(1<<CS11);
	TCCR1B = TCCR1B & ~(1<<CS12);	
	//e enabel globalinterrupt
	sei();
}
void Timer1_Inverting_TOP_at_OCR1A()
{
	/* TCNT1 starts from 0X0000 goes upto TOP and restarts from 0X00*/
	/* Mode of operation:
	WGM1[3:0] --> 0101 --	TOP--> 0X00FF
	WGM1[3:0] --> 0110 --	TOP--> 0x01FF
	WGM1[3:0] --> 0111 --	TOP--> 0x03FF
	WGM1[3:0] --> 1110 --	TOP--> ICR1
	WGM1[3:0] --> 1111 --	TOP--> OCR1A	*/	
	// we take OCR1A for custom frequency and OCR1B for PWM on time(duty cycle)	
	// choose WGM1[3:0] --> 1111 for OCR1A as TOP for custom frequency
	TCCR1A = TCCR1A | (1<<WGM10);
	TCCR1A = TCCR1A | (1<<WGM11);
	TCCR1B = TCCR1B | (1<<WGM12);
	TCCR1B = TCCR1B | (1<<WGM13);
	// for ninverting on  OC1B we use 11 for and COM1B[1:0]	
	// COM1B[1](bit5) from TCCR1A, COM1B[0](bit4) from TCCR1A
	TCCR1A = TCCR1A | (1<<COM1B0);
	TCCR1A = TCCR1A | (1<<COM1B1);
	// Next we set values for OCR1A and OCR1B
	// Since, TCNT1 goes till OCR1A, we can choose OCR1B to any value below OCR1A
	OCR1A = 0x7869; // for freqeuncy
	OCR1B = 0x1A20; // for pwm duty cylc
	// Enable interrupt when count reaches the overflow value
	TIMSK1 |= (1<<TOV1);
	// Enable interrupt when count reaches the OCR1B
	TIMSK1 |= (1<<OCF1B);
	// start timer by setting the clock prescalar
	// SAME AS from I/O clock
	// same-- CS1[2:0] === 001
	// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
	TCCR1B = TCCR1B | (1<<CS10);
	TCCR1B = TCCR1B & ~(1<<CS11);
	TCCR1B = TCCR1B & ~(1<<CS12);
	//e enabel globalinterrupt
	sei();
}
void Timer1_OC1A_Square()
{
	/* TCNT1 starts from 0X0000 goes upto TOP and restarts from 0X00*/
	/* Mode of operation:
	WGM1[3:0] --> 0101 --	TOP--> 0X00FF
	WGM1[3:0] --> 0110 --	TOP--> 0x01FF
	WGM1[3:0] --> 0111 --	TOP--> 0x03FF
	WGM1[3:0] --> 1110 --	TOP--> ICR1
	WGM1[3:0] --> 1111 --	TOP--> OCR1A	*/	
	// we take OCR1A for custom frequency 
	// choose WGM1[3:0] --> 1111 for OCR1A as TOP for custom frequency
	TCCR1A = TCCR1A | (1<<WGM10);
	TCCR1A = TCCR1A | (1<<WGM11);
	TCCR1B = TCCR1B | (1<<WGM12);
	TCCR1B = TCCR1B | (1<<WGM13);	
    // here we set COM1B[1:0] as 01 for toggling of OC1A
    // which is reflected in PB1
    // COM1B[1](bit5) from TCCR1A, COM1B[0](bit4) from TCCR1A
    TCCR1A = TCCR1A & ~(1<<5);
    TCCR1A = TCCR1A | (1<<4);
    OCR1A = 0x7869; // for freqeuncy
	// start timer by setting the clock prescalar
	// SAME AS from I/O clock
	// same-- CS1[2:0] === 001
	// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
	TCCR1B = TCCR1B | (1<<CS10);
	TCCR1B = TCCR1B & ~(1<<CS11);
	TCCR1B = TCCR1B & ~(1<<CS12);
	//enabled global interrupt
    sei();
}
void Timer1_FastPWMGeneration(uint32_t on_time_us, uint32_t off_time_us)
{
	uint32_t total_time = on_time_us + off_time_us;	
	/* TCNT1 starts from 0X0000 goes upto TOP and restarts from 0X00*/
	/* Mode of operation:
		WGM1[3:0] --> 0101 --	TOP--> 0X00FF
		WGM1[3:0] --> 0110 --	TOP--> 0x01FF
		WGM1[3:0] --> 0111 --	TOP--> 0x03FF
		WGM1[3:0] --> 1110 --	TOP--> ICR1
		WGM1[3:0] --> 1111 --	TOP--> OCR1A
	*/	
	// we take OCR1A for custom frequency and OCR1B for PWM on time(duty cycle)	
	// choose WGM1[3:0] --> 1111 for OCR1A as TOP for custom frequency
	TCCR1A = TCCR1A | (1<<WGM10);
	TCCR1A = TCCR1A | (1<<WGM11);
	TCCR1B = TCCR1B | (1<<WGM12);
	TCCR1B = TCCR1B | (1<<WGM13);	
	// COM1B[1](bit5) from TCCR1A, COM1B[0](bit4) from TCCR1A
	TCCR1A = TCCR1A | (1<<COM1B0);
	TCCR1A = TCCR1A | (1<<COM1B1);
	if(total_time <4)
	{
		// if total_time <= 3us -- so we stop clock		
		OCR1A = 0;
		OCR1B = 0;
		// start timer by setting the clock prescalar
		//  use the same clock from I/O clock
		//  CS1[2:0] === 001
		// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
		TCCR1B = TCCR1B & ~(1<<0);
		TCCR1B = TCCR1B & ~(1<<1);
		TCCR1B = TCCR1B & ~(1<<2);
	}
	else if((3 < total_time)  && (total_time <= 4000))
	{
		OCR1A = ((total_time * 16) >> 0) - 1;
		OCR1B = ((on_time_us * 16) >> 0) - 1;
		// start timer by setting the clock prescalar
		//  use the same clock from I/O clock
		//  CS1[2:0] === 001
		// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
		TCCR1B = TCCR1B | (1<<0);
		TCCR1B = TCCR1B & ~(1<<1);
		TCCR1B = TCCR1B & ~(1<<2);
	}
	else if((4000 < total_time)  && (total_time <= 32000))
	{
		OCR1A = ((total_time * 16) >> 3) - 1;
		OCR1B = ((on_time_us * 16) >> 3) - 1;
		// start timer by setting the clock prescalar
		//  dived by 8 from I/O clock
		//  CS1[2:0] === 010
		// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
		TCCR1B = TCCR1B & ~(1<<0);
		TCCR1B = TCCR1B | (1<<1);
		TCCR1B = TCCR1B & ~(1<<2);
	}
	else if((32000 < total_time)  && (total_time <= 260000))
	{
		OCR1A = ((total_time * 16) >> 6) - 1;
		OCR1B = ((on_time_us * 16) >> 6) - 1;
		// start timer by setting the clock prescalar
		//  dived by 64 from I/O clock
		//  CS1[2:0] === 011
		// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
		TCCR1B = TCCR1B | (1<<0);
		TCCR1B = TCCR1B | (1<<1);
		TCCR1B = TCCR1B & ~(1<<2);		
	}
	else if((260000 < total_time)  && (total_time <= 1000000))
	{
		OCR1A = ((total_time * 16) >> 8) - 1;
		OCR1B = ((on_time_us * 16) >> 8) - 1;
		// start timer by setting the clock prescalar
		//  divide by256 from I/O clock
		//  CS1[2:0] === 100
		// CS1[2](bit2) from TCCR1B,CS1[1](bit1) from TCCR1B,CS1[0](bit0) from TCCR1B
		TCCR1B = TCCR1B & ~(1<<0);
		TCCR1B = TCCR1B & ~(1<<1);
		TCCR1B = TCCR1B | (1<<2);		
	}
	else if(total_time > 1000000)
	{
		// dont' cross more than 1s
	}
}
void PWMGeneration(double duty_cycle_percent,uint32_t freqeuncy)
{
	double total_time_us = (1000000.0/freqeuncy);
	double on_time_us = (duty_cycle_percent/100.0) * total_time_us;
	if (on_time_us<1.0)
	{
		on_time_us = 1;
	}	
	// max time = 1S -- min freqency = 1 Hz
	//  min time = 4us -- max frequency = 250000 = 250khz
	Timer1_FastPWMGeneration(on_time_us, total_time_us - on_time_us);
}
int main(void)
{
	DDRB = DDRB | (1<<1) | (1<<2);
	// Timer1_NonInverting_TOP_at_MAX();
	// Timer1_Inverting_TOP_at_MAX();
    Timer1_NonInverting_TOP_at_OCR1A();
    // Timer1_Inverting_TOP_at_OCR1A();
    // Timer1_OC1A_Square();
    // PWMGeneration(12, 1000);
    while(1)
    {
    }
}
ISR(TIMER1_OVF_vect)
{
} 
ISR(TIMER1_COMPA_vect)
{
}
ISR(TIMER1_COMPB_vect)
{
}