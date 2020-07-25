#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void externalInterruptINT0()
{
	// making PD2 as input for INT0, though not neeced
	DDRD &= ~(1<<2);
	// enabling the internal pull-up register for PD2 for INT0
	PORTD |= (1<<2);

 	// making EICRA's ISC01 and ISC00 as 10 for falling edge detection at INT0
	EICRA |= (1<<ISC01);
	EICRA &= ~(1<<ISC00);
	// making EIMSK's INT0 as 1 to enable External Interrput Request for INT0
	EIMSK |= (1<<INT0);
	
	// Enabling global Interrupts
	sei();	
	
}
void externalInterruptINT1()
{
	// making PD3 as input for INT1, though not neeced
	DDRD &= ~(1<<3);
	// enabling the internal pull-up register for PD3 for INT1
	PORTD |= (1<<3);

 	// making EICRA's ISC21 and ISC20 as 11 for rising edge detection at INT1
	EICRA |= ((1<<ISC11) | (1<<ISC10));
	// making EIMSK's INT2 as 1 to enable External Interrput Request for INT1
	EIMSK |= (1<<INT1);
	
	// Enabling global Interrupts
	sei();	
	
}
int main(void)
{
	DDRC |= 0X03; // making PC[1:0] as output for led
	PORTC &= 0XFC; // PC[1:0] is made 0
	externalInterruptINT0();
	externalInterruptINT1();
	while(1)
	{
	}
	return 0;
}

ISR(INT0_vect)
{
	if((EIFR & (1<<INTF0)) != 0)	// INT0 interrupt as occured
	{		
		//toggle Led at pinc 0
		PINC |= (1<<0);
	}
}
ISR(INT1_vect)
{
	if((EIFR & (1<<INTF1)) != 0)	// INT1 interrupt as occured
	{
		//toggle Led at pinc 1
		PINC |= (1<<1);
	}
}
