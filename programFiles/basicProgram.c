#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1<<5);

	while(1)
	{
		PORTB |= (1<<5);;
		_delay_ms(100);
		PORTB &= ~(1<<5);;
		_delay_ms(100);
	}
	return 0;
}
