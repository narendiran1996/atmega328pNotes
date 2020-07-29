#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
void Timer0_NonInverting_TOP_at_MAX()
{
	/* TCNT0 starts from 0x00 to TOP and reaches to 0X00 \
	Here, TOP is defined by WGM0[2] bit 
	0 -- TOP = 0xFF
	1 -- TOP = OCR0A
	for top begin max we select WGM0[2:0] = 011	*/
	/* The frequnecy of PWM is fixed based just on the prescalare 
	becase, the TCN0 reaches from 0X00 to 0XFF
	hence, Based on the prescalling possiblily{1,8,64,256,1024} 
	we have just 5 Frequnecies possible */
	/* But, we get two PWM's using OCR0A and OCR0B 
	A) choosing,  10 - Clear OC0A on compare match. Set OC0A at BOTTOM. 
	will lead to on-time = OCR0A
	B) choosing,  11 - Set OC0A on compare match. Clear OC0A at BOTTOM.
	 will lead to off-time = OCR0A
	A) choosing,  10 - Clear OC0B on compare match. Set OC0B at BOTTOM.
	 will lead to on-time = OCR0B
	B) choosing,  11 - Set OC0B on compare match. Clear OC0B at BOTTOM.
	 will lead to off-time = OCR0B*/
	// MOde of operation to fast_pwm_top_max Mode -- WGM0[2:0] === 011
	// WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
	TCCR0A = TCCR0A | (1<<0);
	TCCR0A = TCCR0A | (1<<1);
	TCCR0B = TCCR0B & ~(1<<3);	    
    // here we set COM0A[1:0] as 10 for non-inverting
	// here we set COM0B[1:0] as 10 for non-inverting	
	// which is reflected in PD6
	// COM0A[1](bit7) from TCCR0A, COM0A[0](bit6) from TCCR0A
	TCCR0A = TCCR0A | (1<<7);
	TCCR0A = TCCR0A & ~(1<<6);	
	// which is reflected in PD65
	// COM0B[1](bit5) from TCCR0A, COM0B[0](bit4) from TCCR0A
	TCCR0A = TCCR0A | (1<<5);
	TCCR0A = TCCR0A & ~(1<<4);
    // Enable Interrupt when TCN0 overflows TOP - here 0xFF
	//  TOV0 bit is enabled
	TIMSK0 = TIMSK0 | (1<<0);	
	/* we use OCF0A flag - which is set at every time TCN0 reaches OCR0A 
	   here we clear led(PC1),  so that we obtain the PWM when TCN0 reaches OCR0A*/
	TIMSK0 = TIMSK0 | (1<<1);
	/* we use OCF0B flag - which is set at every time TCN0 reaches OCR0B 
	   here we clear led(PC2),  so that we obtain the PWM when TCN0 reaches OCR0B*/
	TIMSK0 = TIMSK0 | (1<<2);    		
	// Next we set values for OCR0A and OCR0B
	/* Since, TCNT0 goes till max(0xFF), we can choose OCR0A
	 and OCR0B to any value below max(0xFFF)*/
	OCR0A = 0x19; // for 10% duty clcle
	OCR0B = 0xC0; // for 75% duty clcle	
    // start the timer by selecting the prescalr
	//  use the same clock from I/O clock
	//  CS0[2:0] === 001
	// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
	TCCR0B = TCCR0B | (1<<0);
	TCCR0B = TCCR0B & ~(1<<1);
	TCCR0B = TCCR0B & ~(1<<2);	
	//enabled global interrupt
	sei();
}
void Timer0_Inverting_TOP_at_MAX()
{
    // MOde of operation to fast_pwm_top_max Mode -- WGM0[2:0] === 011
	// WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
	TCCR0A = TCCR0A | (1<<0);
	TCCR0A = TCCR0A | (1<<1);
	TCCR0B = TCCR0B & ~(1<<3);	    
    // here we set COM0A[1:0] as 11 for inverting
	// here we set COM0B[1:0] as 11 for inverting	
	// which is reflected in PD6
	// COM0A[1](bit7) from TCCR0A, COM0A[0](bit6) from TCCR0A
	TCCR0A = TCCR0A | (1<<7);
	TCCR0A = TCCR0A | (1<<6);	
	// which is reflected in PD65
	// COM0B[1](bit5) from TCCR0A, COM0B[0](bit4) from TCCR0A
	TCCR0A = TCCR0A | (1<<5);
	TCCR0A = TCCR0A | (1<<4);
    // Enable Interrupt when TCN0 overflows TOP - here 0xFF
	//  TOV0 bit is enabled
	TIMSK0 = TIMSK0 | (1<<0);	
	/* we use OCF0A flag - which is set at every time TCN0 reaches OCR0A 
	   here we clear led(PC1),  so that we obtain the PWM when TCN0 reaches OCR0A*/
	TIMSK0 = TIMSK0 | (1<<1);
	/* we use OCF0B flag - which is set at every time TCN0 reaches OCR0B 
	   here we clear led(PC2),  so that we obtain the PWM when TCN0 reaches OCR0B*/
	TIMSK0 = TIMSK0 | (1<<2);    		
	// Next we set values for OCR0A and OCR0B
	/* Since, TCNT0 goes till max(0xFF), we can choose OCR0A 
	and OCR0B to any value below max(0xFFF)*/
	OCR0A = 0x19; // for 10% duty clcle
	OCR0B = 0xC0; // for 75% duty clcle  
    // start the timer by selecting the prescalr
	//  use the same clock from I/O clock
	//  CS0[2:0] === 001
	// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
	TCCR0B = TCCR0B | (1<<0);
	TCCR0B = TCCR0B & ~(1<<1);
	TCCR0B = TCCR0B & ~(1<<2);	
	//enabled global interrupt
	sei();
}
void Timer0_NonInverting_TOP_at_OCR0A()
{
    // MOde of operation to fast_pwm_top_max Mode -- WGM0[2:0] === 111
	// WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
	TCCR0A = TCCR0A | (1<<0);
	TCCR0A = TCCR0A | (1<<1);
	TCCR0B = TCCR0B | (1<<3);	
	// here we set COM0B[1:0] as 10 for non-inverting
	// which is reflected in PD5
	// COM0B[1](bit5) from TCCR0A, COM0B[0](bit4) from TCCR0A
	TCCR0A = TCCR0A | (1<<5);
	TCCR0A = TCCR0A & ~(1<<4);
	// Next we set values for OCR0A and OCR0B
	// Since, TCNT0 goes till OCR0A, we can choose OCR0B to any value below OCR0A
	OCR0A = 0x70; // for freqeuncy
	OCR0B = 0x60; // for pwm duty cylc
	// start the timer by selecting the prescalr
	//  use the same clock from I/O clock
	//  CS0[2:0] === 001
	// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
	TCCR0B = TCCR0B | (1<<0);
	TCCR0B = TCCR0B & ~(1<<1);
	TCCR0B = TCCR0B & ~(1<<2);
	//enabled global interrupt
	sei();
}
void Timer0_Inverting_TOP_at_OCR0A()
{
    // MOde of operation to fast_pwm_top_max Mode -- WGM0[2:0] === 111
    // WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
    TCCR0A = TCCR0A | (1<<0);
    TCCR0A = TCCR0A | (1<<1);
    TCCR0B = TCCR0B | (1<<3);	
    // here we set COM0B[1:0] as 11 for inverting
    // which is reflected in PD5
    // COM0B[1](bit5) from TCCR0A, COM0B[0](bit4) from TCCR0A
    TCCR0A = TCCR0A | (1<<5);
    TCCR0A = TCCR0A | (1<<4);
    // Next we set values for OCR0A and OCR0B
    // Since, TCNT0 goes till OCR0A, we can choose OCR0B to any value below OCR0A
    OCR0A = 0x70; // for freqeuncy
    OCR0B = 0x60; // for pwm duty cylc
    // start the timer by selecting the prescalr
    //  use the same clock from I/O clock
    //  CS0[2:0] === 001
    // CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
    TCCR0B = TCCR0B | (1<<0);
    TCCR0B = TCCR0B & ~(1<<1);
    TCCR0B = TCCR0B & ~(1<<2);
    //enabled global interrupt
    sei();
}
void Timer0_OC0A_Square()
{
    // MOde of operation to fast_pwm_top_max Mode -- WGM0[2:0] === 111
    // WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
    TCCR0A = TCCR0A | (1<<0);
    TCCR0A = TCCR0A | (1<<1);
    TCCR0B = TCCR0B | (1<<3);	
    // here we set COM0B[1:0] as 01 for toggling of OC0A
    // which is reflected in PD6
    // COM0A[1](bit7) from TCCR0A, COM0A[0](bit6) from TCCR0A
    TCCR0A = TCCR0A & ~(1<<7);
    TCCR0A = TCCR0A | (1<<6);
    // Next we set values for OCR0A and OCR0B
    // Since, TCNT0 goes till OCR0A, we can choose OCR0B to any value below OCR0A
    OCR0A = 0x70; // for freqeuncy
    // start the timer by selecting the prescalr
    //  use the same clock from I/O clock
    //  CS0[2:0] === 001
    // CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
    TCCR0B = TCCR0B | (1<<0);
    TCCR0B = TCCR0B & ~(1<<1);
    TCCR0B = TCCR0B & ~(1<<2);
    //enabled global interrupt
    sei();
}
void Timer0_FastPWMGeneration(uint32_t on_time_us, uint32_t off_time_us)
{
	uint32_t total_time = on_time_us + off_time_us;		
	// MOde of operation to fast_pwm_top_max Mode -- WGM0[2:0] === 111
	// WGM0[2](bit3) from TCCR0B, WGM0[1](bit1)  from TCCR0A, WGM0[0](bit0)  from TCCR0A
	TCCR0A = TCCR0A | (1<<0);
	TCCR0A = TCCR0A | (1<<1);
	TCCR0B = TCCR0B | (1<<3);	
	// which is reflected in PD5
	// COM0B[1](bit5) from TCCR0A, COM0B[0](bit4) from TCCR0A
	TCCR0A = TCCR0A | (1<<5);
	TCCR0A = TCCR0A & ~(1<<4);	
	if(total_time <=3)
	{
		// if total_time <= 3us -- so we stop clock		
		OCR0A = 0;
		// start timer by setting the clock prescalar
		//  use the same clock from I/O clock
		//  CS0[2:0] === 001
		// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
		TCCR0B = TCCR0B & ~(1<<0);
		TCCR0B = TCCR0B & ~(1<<1);
		TCCR0B = TCCR0B & ~(1<<2);
	}
	else if((3 < total_time)  && (total_time <= 16))
	{
		OCR0A = ((total_time * 16) >> 0) - 1;
		OCR0B = ((on_time_us * 16) >> 0) - 1;
		// start timer by setting the clock prescalar
		//  use the same clock from I/O clock
		//  CS0[2:0] === 001
		// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
		TCCR0B = TCCR0B | (1<<0);
		TCCR0B = TCCR0B & ~(1<<1);
		TCCR0B = TCCR0B & ~(1<<2);
	}
	else if((16 < total_time)  && (total_time <= 128))
	{
		OCR0A = ((total_time * 16) >> 3) - 1;
		OCR0B = ((on_time_us * 16) >> 3) - 1;
		// start timer by setting the clock prescalar
		//  dived by 8 from I/O clock
		//  CS0[2:0] === 010
		// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
		TCCR0B = TCCR0B & ~(1<<0);
		TCCR0B = TCCR0B | (1<<1);
		TCCR0B = TCCR0B & ~(1<<2);
	}
	else if((128 < total_time)  && (total_time <= 1024))
	{
		OCR0A = ((total_time * 16) >> 6) - 1;
		OCR0B = ((on_time_us * 16) >> 6) - 1;
		// start timer by setting the clock prescalar
		//  dived by 64 from I/O clock
		//  CS0[2:0] === 011
		// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
		TCCR0B = TCCR0B | (1<<0);
		TCCR0B = TCCR0B | (1<<1);
		TCCR0B = TCCR0B & ~(1<<2);		
	}
	else if((1024 < total_time)  && (total_time <= 4096))
	{
		OCR0A = ((total_time * 16) >> 8) - 1;
		OCR0B = ((on_time_us * 16) >> 8) - 1;
		// start timer by setting the clock prescalar
		//  divide by256 from I/O clock
		//  CS0[2:0] === 100
		// CS0[2](bit2) from TCCR0B,CS0[1](bit1) from TCCR0B,CS0[0](bit0) from TCCR0B
		TCCR0B = TCCR0B & ~(1<<0);
		TCCR0B = TCCR0B & ~(1<<1);
		TCCR0B = TCCR0B | (1<<2);		
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
	Timer0_FastPWMGeneration(on_time_us, total_time_us - on_time_us);
}
int main(void)
{
	DDRD = DDRD | (1<<6) | (1<<5);
	// Timer0_NonInverting_TOP_at_MAX();
	// Timer0_Inverting_TOP_at_MAX();
    // Timer0_NonInverting_TOP_at_OCR0A();
    // Timer0_Inverting_TOP_at_OCR0A();
    // Timer0_OC0A_Square();
    PWMGeneration(12, 1000);
    while(1)
    {
    }
}
ISR(TIMER0_OVF_vect)
{
} 
ISR(TIMER0_COMPA_vect)
{
}
ISR(TIMER0_COMPB_vect)
{
}