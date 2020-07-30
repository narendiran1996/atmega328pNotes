#include "UARTLCDinclude.h"

void UARTLCD_init(void)
{
	// Selecting mode for usart operation 
	// UMSEL0[1:0] from UCSR0C -- 00 - Asynchrous USART mode
	UCSR0C &= ~(1<<UMSEL00);
	UCSR0C &= ~(1<<UMSEL01);

	// Selecting Parity - with no parity
	// UPM0[1:0] from UCSR0C -- 00 - Disabled
	UCSR0C &= ~(1<<UPM00);
	UCSR0C &= ~(1<<UPM01);

	// Selecting stop Bits
	// USBS0 from UCSR0C -- 0 - 1bit
	UCSR0C &= ~(1<<USBS0);

	// Selection characeter size
	// UCSZ0[2:0] -- 011 - 8bit size
	// USCZ0[2] from UCSR0B and USCZ0[1:0] from UCSR0C
	UCSR0B &= ~(1<<UCSZ02);
	UCSR0C |= (1<<UCSZ01);
	UCSR0C |= (1<<UCSZ00);

	/* Let's have a baud rate of 9600 
	UBRR0 = (fosc / (16*BAUD)) - 1 */
	/* We have fosc = 16MZH so UBRR0 = ( 16000000 / (16*9600)) -1 = 0X69 */
	UBRR0H = 0x00;
	UBRR0L = 0x69;

	// Enabling Transmitter 
	UCSR0B |= (1<<TXEN0);
	_delay_ms(500);
}
void UARTLCD_send_data(uint8_t data_)
{
	//cHECKING if transmitet buffer is empty
	while((UCSR0A & (1<<UDRE0)) == 0x00){};

	UDR0 = data_;
}
void UARTLCD_send_cmd(uint8_t cmd_)
{
	UARTLCD_send_data(254);
	UARTLCD_send_data(cmd_);
}
void UARTLCD_clear(void)
{	
	// 0x01 for clearing display
	UARTLCD_send_cmd(0x01);	
}
void UARTLCD_home(void)
{
	// 0000_001x for moving cursor to inital position and moving DRAM address to 00H
	UARTLCD_send_cmd(0x02);
}
void UARTLCD_print_left(void)
{
	// moving left
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x04
	UARTLCD_send_cmd(0x04);
}
void UARTLCD_print_right(void)
{
	// moving left
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x06
	UARTLCD_send_cmd(0x06);
}
void UARTLCD_shift_left(void)
{
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x06
	UARTLCD_send_cmd(0x07);
}
void UARTLCD_shift_right(void)
{
	/* Works when there is character before the first character */
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x06
	UARTLCD_send_cmd(0x05);
}
void UARTLCD_display_off(void)
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_10xx = 0x0F;
	UARTLCD_send_cmd(0x08);
}
void UARTLCD_display_on_cursor_off_blink_off(void)
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1100 = 0x0E;
	UARTLCD_send_cmd(0x0C);
}
void UARTLCD_display_on_cursor_on_blink_off(void)
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1110 = 0x0E;
	UARTLCD_send_cmd(0xE);
}
void UARTLCD_display_on_cursor_off_blink_on(void)
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1101 = 0x0D;
	UARTLCD_send_cmd(0x0D);
}
void UARTLCD_display_on_cursor_on_blink_on(void)
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1111 = 0x0F;
	UARTLCD_send_cmd(0x0F);
}
void UARTLCD_cursor_leftShift(void)
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- 
	// (S/C)(R/L) === 10 -- 
	// (S/C)(R/L) === 11 -- 
	// so we slect D[7:0] = 0b0001_00xx = 0x10;
	UARTLCD_send_cmd(0x010);	
}
void UARTLCD_cursor_rightShift(void)
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- Shift Cursor to Right
	// (S/C)(R/L) === 10 --
	// (S/C)(R/L) === 11 --
	// so we slect D[7:0] = 0b0001_01xx = 0x14;
	UARTLCD_send_cmd(0x014);	
}
void UARTLCD_display_leftShift(void)
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- Shift Cursor to Right
	// (S/C)(R/L) === 10 -- Shift Display to Left
	// (S/C)(R/L) === 11 --
	// so we slect D[7:0] = 0b0001_10xx = 0x18;
	UARTLCD_send_cmd(0x018);
}
void UARTLCD_display_rightShift(void)
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- Shift Cursor to Right
	// (S/C)(R/L) === 10 -- Shift Display to Left
	// (S/C)(R/L) === 11 -- Shift Display to Left
	// so we slect D[7:0] = 0b0001_11xx = 0x1C;
	UARTLCD_send_cmd(0x01C);
}
void UARTLCD_send_string(char *c_string)
{
	while(*c_string != '\0')
	{
		UARTLCD_send_data(*c_string++);
	}
}
void UARTLCD_set_cursor(uint8_t r,uint8_t c)
{
	// r and c starts from 1
	if (r==1)
	{
		UARTLCD_send_cmd(0x80+c-1);
	}
	else if (r==2)
	{
		UARTLCD_send_cmd(0xC0+c-1);
	}
}

void UARTLCD_send_string_l(uint8_t *c_string,uint8_t len)
{
	for (uint8_t i=0;i<len;i++)
	{
		UARTLCD_send_data(c_string[i]);
	}
}

void UARTLCD_print_num(int num,uint8_t dig)
{

	uint8_t buf[16];

	itoa(num, (char *)buf, 10);
	
	UARTLCD_send_string_l(buf,dig);
}
