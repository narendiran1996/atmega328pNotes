#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void pinChangeInterrupt_PCINT20()
{
	// making PD4 as input for PCI20
	DDRD &= ~(1<<4);
	// enabling the internal pull-up register for PD4 for PCI20
	PORTD |= (1<<4);

 	// Selecting the PCINT20 for PCI2 intterupt
	PCMSK2 |= (1<<PCINT20);
	// Enabling the PCI2 interupt
	PCICR |= (1<<PCIE2);
	
	// Enabling global Interrupts
	sei();	
	
}
int main(void)
{
	DDRC |= 0X01; // making PC0 as output for led
	PORTC &= 0XFE; // PC0 is made 0
	pinChangeInterrupt_PCINT20();
	while(1)
	{

	}
	return 0;
}

ISR(PCINT2_vect)
{
	if((PCIFR & (1<<PCIF2)) != 0)	// PCI2 interrupt as occured
	{		
		//toggle Led at pinc 0
		PINC |= (1<<0);

	}
}
