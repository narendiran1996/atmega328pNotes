#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
uint16_t ADC_SingleConversion(uint8_t channel_no)
{
	DDRC &= ~(1<<channel_no);
	// Selecting Voltage Referece
	// Lets use AREF pin
	// REFS[1:0] -- 00
	ADMUX &= ~(1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	// Selecting the Presentation of ADC output
	// Right adjust - ADLAR == 0
	ADMUX &= ~(1<<ADLAR);
	// SELECTINT the channel for ADC
	// LET'S select channel_no
	// MUX[3:0]&0xF0 | channel_no
	ADMUX = (ADMUX & 0XF0) | channel_no;
	// for single conversion - disabling ADC auto trigger
	// ADATE == 0
	ADCSRA &= ~(1<<ADATE);
	// disable the interrrupt by disbaling ADIE bit
	// ADIE == 0
	ADCSRA &= ~(1<<ADIE);
	//  Prescaler be 64 so that we get 8Mhz/64 = 125kHz
	// ADPS[2:0] -- 110
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1);
	ADCSRA &= ~(1<<ADPS0);
	// ENABLING adc
	ADCSRA |= (1<<ADEN);
	// STARTING CONVERSIOn
	ADCSRA |= (1<<ADSC);	
		// since single conversion, we can check start conversion bit
	while((ADCSRA & (1<<ADSC)))
	{		
	}
	// RESSETTING THE Flag
	// ADCSRA |= (1<<ADIF);
	return ADC;
}
volatile uint16_t free_running_value=0;
void ADC_FreeRunningInit(uint8_t channel_no)
{
	DDRC &= ~(1<<channel_no);
	// Selecting Voltage Referece
	// Lets use AREF pin
	// REFS[1:0] -- 00
	ADMUX &= ~(1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	// Selecting the Presentation of ADC output
	// Right adjust - ADLAR == 0
	ADMUX &= ~(1<<ADLAR);
	// SELECTINT the channel for ADC
	// LET'S select channel_no
	// MUX[3:0]&0xF0 | channel_no
	ADMUX = (ADMUX & 0XF0) | channel_no;
	// Select the Auto Trigger source
	// for free running, use 000 for ADTS[2:0] in ADCSRB 
	ADCSRB &= ~(1<<ADTS2);
	ADCSRB &= ~(1<<ADTS1);
	ADCSRB &= ~(1<<ADTS0);
	// for free runing conversion - enable ADC auto trigger
	// ADATE == 1
	ADCSRA |= (1<<ADATE);
	// enable the interrrupt by enabling ADIE bit
	// ADIE == 1
	ADCSRA |= (1<<ADIE);
	//  Prescaler be 64 so that we get 8Mhz/64 = 125kHz
	// ADPS[2:0] -- 110
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1);
	ADCSRA &= ~(1<<ADPS0);
	// ENABLING adc
	ADCSRA |= (1<<ADEN);
	// STARTING CONVERSIOn
	ADCSRA |= (1<<ADSC);	
	sei();
}
int main(void)
{
	ADC_FreeRunningInit(5);
    while(1)
    {
		// ADC_SingleConversion(5);
		// _delay_ms(100);
    }
}
ISR(ADC_vect)
{
	free_running_value = ADC;
	// ADCSRA |= (1<<ADIF);
}