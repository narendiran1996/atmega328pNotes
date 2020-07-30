#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>


#include "UARTLCDinclude.c"

int main(void)
{
	UARTLCD_init();
	UARTLCD_send_string("workIng?**--");
	UARTLCD_display_on_cursor_on_blink_on();
	_delay_ms(1000);
	UARTLCD_display_rightShift();
    while (1) 
    {
		
    }
}