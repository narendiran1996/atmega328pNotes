#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void AnalogCompartorInit()
{
	/* Disabling the Analog Comparator Multiplexer Enable bit
	 so that AIN1 is selected as positive input */
	ADCSRB &= ~(1<<ACME);
	/* Disabling the Analog Comparator Bandgap Select bit 
	so that AIN0 is selected as negative input */
	ACSR &= ~(1<<ACBG);
	// Choosing the interrupt mode to toggle ACO bit 
	// By selecting 00 to ACIS[1:0]
	ACSR &= ~(1<<ACIS1);
	ACSR &= ~(1<<ACIS0);
	// Enabling the Analog Comparator interrupt Enable to see the output
	ACSR |= (1<<ACIE);
	// enabling the Analog Comparator by clearing the Analog Comparator Disable bit
	ACSR &= ~(1<<ACD);
	sei();
}

int main(void)
{
	// making the AIN0(PD6) and AIN1(PD7) as input
	DDRD &= ~(1<<6);
	DDRD &= ~(1<<7);	
	// making PC0 as output - to show output
	DDRC |= (1<<0);	
    while(1)
    {
    }
}
ISR(ANALOG_COMP_vect)
{
    PINC |= (1<<0);
}