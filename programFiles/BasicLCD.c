#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>


#include "LCDinclude.c"

int main(void)
{
	LCD_init();
	LCD_send_string("workIng?**--");
	LCD_display_on_cursor_on_blink_on();
	_delay_ms(1000);
	LCD_display_rightShift();
    while (1) 
    {
		
    }
}