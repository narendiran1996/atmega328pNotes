#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	DDRC |= (1<<0);
	PORTC &= ~(1<<0);

    while(1)
    {
		PORTC |= (1<<0);
		_delay_ms(1000);
		PORTC &= ~(1<<0);
		_delay_ms(1000);
    }
}
