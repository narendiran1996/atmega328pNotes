#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
void Timer2_NonInverting_TOP_at_MAX()
{
	// MOde of operation to fast_pwm_top_max Mode -- WGM2[2:0] === 011
	// WGM2[2](bit3) from TCCR2B, WGM2[1](bit1)  from TCCR2A, WGM2[0](bit0)  from TCCR2A
	TCCR2A = TCCR2A | (1<<0);
	TCCR2A = TCCR2A | (1<<1);
	TCCR2B = TCCR2B & ~(1<<3);	

	// here we set COM2A[1:0] as 10 for non-inverting
	// here we set COM2B[1:0] as 10 for non-inverting

	// which is reflected in PB3
	// COM2A[1](bit7) from TCCR2A, COM2A[0](bit6) from TCCR2A
	TCCR2A = TCCR2A | (1<<7);
	TCCR2A = TCCR2A & ~(1<<6);

	// which is reflected in PB35
	// COM2B[1](bit5) from TCCR2A, COM2B[0](bit4) from TCCR2A
	TCCR2A = TCCR2A | (1<<5);
	TCCR2A = TCCR2A & ~(1<<4);

	// Enable Interrupt when TCN0 overflows TOP - here 0xFF
	//  TOV2 bit is enabled
	TIMSK2 = TIMSK2 | (1<<0);

	/* we use OCF0A flag - which is set at every time TCN0 reaches OCR2A 
	here we clear led(PC1),  so that we obtain the PWM when TCN0 reaches OCR2A*/
	TIMSK2 = TIMSK2 | (1<<1);
	/* we use OCF0B flag - which is set at every time TCN0 reaches OCR2B 
	here we clear led(PC2),  so that we obtain the PWM when TCN0 reaches OCR2B*/
	TIMSK2 = TIMSK2 | (1<<2);


	// Next we set values for OCR2A and OCR2B
	// Since, TCNT2 goes till max(0xFF), we can choose OCR2A and OCR2B to any value below max(0xFFF)
	OCR2A = 0x19; // for 10% duty clcle
	OCR2B = 0xC0; // for 75% duty clcle


	// start the timer by selecting the prescalr
	//  use the same clock from I/O clock
	//  CS2[2:0] === 001
	// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
	TCCR2B = TCCR2B | (1<<0);
	TCCR2B = TCCR2B & ~(1<<1);
	TCCR2B = TCCR2B & ~(1<<2);

	//enabled global interrupt
	sei();
}
void Timer2_Inverting_TOP_at_MAX()
{
	// MOde of operation to fast_pwm_top_max Mode -- WGM2[2:0] === 011
	// WGM2[2](bit3) from TCCR2B, WGM2[1](bit1)  from TCCR2A, WGM2[0](bit0)  from TCCR2A
	TCCR2A = TCCR2A | (1<<0);
	TCCR2A = TCCR2A | (1<<1);
	TCCR2B = TCCR2B & ~(1<<3);	

	// here we set COM2A[1:0] as 11 for inverting
	// here we set COM2B[1:0] as 11 for inverting

	// which is reflected in PB3
	// COM2A[1](bit7) from TCCR2A, COM2A[0](bit6) from TCCR2A
	TCCR2A = TCCR2A | (1<<7);
	TCCR2A = TCCR2A | (1<<6);

	// which is reflected in PB35
	// COM2B[1](bit5) from TCCR2A, COM2B[0](bit4) from TCCR2A
	TCCR2A = TCCR2A | (1<<5);
	TCCR2A = TCCR2A | (1<<4);

	// Enable Interrupt when TCN0 overflows TOP - here 0xFF
	//  TOV2 bit is enabled
	TIMSK2 = TIMSK2 | (1<<0);

	/* we use OCF0A flag - which is set at every time TCN0 reaches OCR2A 
		here we clear led(PC1),  so that we obtain the PWM when TCN0 reaches OCR2A*/
	TIMSK2 = TIMSK2 | (1<<1);
	/* we use OCF0B flag - which is set at every time TCN0 reaches OCR2B 
		here we clear led(PC2),  so that we obtain the PWM when TCN0 reaches OCR2B*/
	TIMSK2 = TIMSK2 | (1<<2);

		    
	// Next we set values for OCR2A and OCR2B
	// Since, TCNT2 goes till max(0xFF), we can choose OCR2A and OCR2B to any value below max(0xFFF)
	OCR2A = 0x19; // for 10% duty clcle
	OCR2B = 0xC0; // for 75% duty clcle


	// start the timer by selecting the prescalr
	//  use the same clock from I/O clock
	//  CS2[2:0] === 001
	// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
	TCCR2B = TCCR2B | (1<<0);
	TCCR2B = TCCR2B & ~(1<<1);
	TCCR2B = TCCR2B & ~(1<<2);

	//enabled global interrupt
	sei();
}
void Timer2_NonInverting_TOP_at_OCR2A()
{
	// MOde of operation to fast_pwm_top_max Mode -- WGM2[2:0] === 111
	// WGM2[2](bit3) from TCCR2B, WGM2[1](bit1)  from TCCR2A, WGM2[0](bit0)  from TCCR2A
	TCCR2A = TCCR2A | (1<<0);
	TCCR2A = TCCR2A | (1<<1);
	TCCR2B = TCCR2B | (1<<3);	

	// here we set COM2B[1:0] as 10 for non-inverting
	// which is reflected in PD3
	// COM2B[1](bit5) from TCCR2A, COM2B[0](bit4) from TCCR2A
	TCCR2A = TCCR2A | (1<<5);
	TCCR2A = TCCR2A & ~(1<<4);

	// Next we set values for OCR2A and OCR2B
	// Since, TCNT2 goes till OCR2A, we can choose OCR2B to any value below OCR2A
	OCR2A = 0x70; // for freqeuncy
	OCR2B = 0x60; // for pwm duty cylc

	// start the timer by selecting the prescalr
	//  use the same clock from I/O clock
	//  CS2[2:0] === 001
	// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
	TCCR2B = TCCR2B | (1<<0);
	TCCR2B = TCCR2B & ~(1<<1);
	TCCR2B = TCCR2B & ~(1<<2);

	//enabled global interrupt
	sei();
}
void Timer2_Inverting_TOP_at_OCR2A()
{
		
	// MOde of operation to fast_pwm_top_max Mode -- WGM2[2:0] === 111
	// WGM2[2](bit3) from TCCR2B, WGM2[1](bit1)  from TCCR2A, WGM2[0](bit0)  from TCCR2A
	TCCR2A = TCCR2A | (1<<0);
	TCCR2A = TCCR2A | (1<<1);
	TCCR2B = TCCR2B | (1<<3);	

	// here we set COM2B[1:0] as 11 for inverting
	// which is reflected in PD3
	// COM2B[1](bit5) from TCCR2A, COM2B[0](bit4) from TCCR2A
	TCCR2A = TCCR2A | (1<<5);
	TCCR2A = TCCR2A | (1<<4);

	// Next we set values for OCR2A and OCR2B
	// Since, TCNT2 goes till OCR2A, we can choose OCR2B to any value below OCR2A
	OCR2A = 0x70; // for freqeuncy
	OCR2B = 0x60; // for pwm duty cylc

	// start the timer by selecting the prescalr
	//  use the same clock from I/O clock
	//  CS2[2:0] === 001
	// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
	TCCR2B = TCCR2B | (1<<0);
	TCCR2B = TCCR2B & ~(1<<1);
	TCCR2B = TCCR2B & ~(1<<2);

	//enabled global interrupt
	sei();
}
void Timer2_OC2A_Square()
{
    
	// MOde of operation to fast_pwm_top_max Mode -- WGM2[2:0] === 111
	// WGM2[2](bit3) from TCCR2B, WGM2[1](bit1)  from TCCR2A, WGM2[0](bit0)  from TCCR2A
	TCCR2A = TCCR2A | (1<<0);
	TCCR2A = TCCR2A | (1<<1);
	TCCR2B = TCCR2B | (1<<3);	

	// here we set COM2B[1:0] as 01 for toggling of OC2A
	// which is reflected in PB3
	// COM2A[1](bit7) from TCCR2A, COM2A[0](bit6) from TCCR2A
	TCCR2A = TCCR2A & ~(1<<7);
	TCCR2A = TCCR2A | (1<<6);

	// Next we set values for OCR2A and OCR2B
	// Since, TCNT2 goes till OCR2A, we can choose OCR2B to any value below OCR2A
	OCR2A = 0x70; // for freqeuncy

	// start the timer by selecting the prescalr
	//  use the same clock from I/O clock
	//  CS2[2:0] === 001
	// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
	TCCR2B = TCCR2B | (1<<0);
	TCCR2B = TCCR2B & ~(1<<1);
	TCCR2B = TCCR2B & ~(1<<2);

	//enabled global interrupt
	sei();
}
void Timer2_FastPWMGeneration(uint32_t on_time_us, uint32_t off_time_us)
{
	uint32_t total_time = on_time_us + off_time_us;
		
	// MOde of operation to fast_pwm_top_max Mode -- WGM2[2:0] === 111
	// WGM2[2](bit3) from TCCR2B, WGM2[1](bit1)  from TCCR2A, WGM2[0](bit0)  from TCCR2A
	TCCR2A = TCCR2A | (1<<0);
	TCCR2A = TCCR2A | (1<<1);
	TCCR2B = TCCR2B | (1<<3);	

	// which is reflected in PD3
	// COM2B[1](bit5) from TCCR2A, COM2B[0](bit4) from TCCR2A
	TCCR2A = TCCR2A | (1<<5);
	TCCR2A = TCCR2A & ~(1<<4);
	
	if(total_time <=3)
	{
		// if total_time <= 3us -- so we stop clock
		
		OCR2A = 0;
		// start timer by setting the clock prescalar
		//  use the same clock from I/O clock
		//  CS2[2:0] === 001
		// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
		TCCR2B = TCCR2B & ~(1<<0);
		TCCR2B = TCCR2B & ~(1<<1);
		TCCR2B = TCCR2B & ~(1<<2);
	}
	else if((3 < total_time)  && (total_time <= 16))
	{
		OCR2A = ((total_time * 16) >> 0) - 1;
		OCR2B = ((on_time_us * 16) >> 0) - 1;
		// start timer by setting the clock prescalar
		//  use the same clock from I/O clock
		//  CS2[2:0] === 001
		// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
		TCCR2B = TCCR2B | (1<<0);
		TCCR2B = TCCR2B & ~(1<<1);
		TCCR2B = TCCR2B & ~(1<<2);
	}
	else if((16 < total_time)  && (total_time <= 128))
	{
		OCR2A = ((total_time * 16) >> 3) - 1;
		OCR2B = ((on_time_us * 16) >> 3) - 1;
		// start timer by setting the clock prescalar
		//  dived by 8 from I/O clock
		//  CS2[2:0] === 010
		// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
		TCCR2B = TCCR2B & ~(1<<0);
		TCCR2B = TCCR2B | (1<<1);
		TCCR2B = TCCR2B & ~(1<<2);
	}
	else if((128 < total_time)  && (total_time <= 1024))
	{
		OCR2A = ((total_time * 16) >> 6) - 1;
		OCR2B = ((on_time_us * 16) >> 6) - 1;
		// start timer by setting the clock prescalar
		//  dived by 64 from I/O clock
		//  CS2[2:0] === 011
		// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
		TCCR2B = TCCR2B | (1<<0);
		TCCR2B = TCCR2B | (1<<1);
		TCCR2B = TCCR2B & ~(1<<2);
		
	}
	else if((1024 < total_time)  && (total_time <= 4096))
	{
		OCR2A = ((total_time * 16) >> 8) - 1;
		OCR2B = ((on_time_us * 16) >> 8) - 1;
		// start timer by setting the clock prescalar
		//  divide by256 from I/O clock
		//  CS2[2:0] === 100
		// CS2[2](bit2) from TCCR2B,CS2[1](bit1) from TCCR2B,CS2[0](bit0) from TCCR2B
		TCCR2B = TCCR2B & ~(1<<0);
		TCCR2B = TCCR2B & ~(1<<1);
		TCCR2B = TCCR2B | (1<<2);
		
	}
	else if(total_time > 4096)
	{
		// dont' cross more than 4.096ms
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
	
	// max time = 4ms -- min freqency = 250 Hz
	//  min time = 4us -- max frequency = 250000 = 250khz
	Timer2_FastPWMGeneration(on_time_us, total_time_us - on_time_us);
}
int main(void)
{
	DDRD = DDRD | (1<<3);
	DDRB = DDRB | (1<<3);

	//Timer2_NonInverting_TOP_at_MAX();
	 //Timer2_Inverting_TOP_at_MAX();
    //Timer2_NonInverting_TOP_at_OCR2A();
     //Timer2_Inverting_TOP_at_OCR2A();
    //Timer2_OC2A_Square();
    PWMGeneration(12, 1000);
    while(1)
    {
    }
}
ISR(TIMER2_OVF_vect)
{
} 
ISR(TIMER2_COMPA_vect)
{
}
ISR(TIMER2_COMPB_vect)
{
}
