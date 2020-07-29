#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>

// connect PC0 TO EN
// connect PC1 TO RS
// connect PC[5:2] TO LCD[7:4]
#define set_En PORTC |= (1<<PORTC0);
#define clear_En PORTC &= ~(1<<PORTC0);

#define set_RS PORTC |= (1<<PORTC1);
#define clear_RS PORTC &= ~(1<<PORTC1);

#define set_MSB(val_) PORTC = (PORTC & 0b11000011) | ((val_ & 0xF0)>>2); // selecting PC5 - PC2 as LCD[7:4] and sending MSB
#define set_LSB(val_) PORTC = (PORTC & 0b11000011) | ((val_ & 0x0F)<<2);  // selecting PC5 - PC2 as LCD[7:4] and sending LSB


void custom_delay(uint8_t del_val)
{
	for (uint8_t i=0; i<100; i++)
	{
		for (uint8_t j=0; j<10; j++)
		{
			asm("nop"); // if not used, this delay will be optimsed by compiler
		}
	}
}
void port_init_LCD()
{
	DDRC = DDRC | 0b00111111;	// PC0-PC5
	PORTC = PORTC & 0b11000000;
}
void LCD_send_cmd(uint8_t cmd_)
{
	set_MSB(cmd_)
	clear_RS
	set_En
	custom_delay(1);
	clear_En
	
	set_LSB(cmd_)
	clear_RS
	set_En
	custom_delay(1);
	clear_En
}
void LCD_send_data(uint8_t data_)
{
	set_MSB(data_)
	set_RS
	set_En
	custom_delay(1);
	clear_En
	
	set_LSB(data_)
	set_RS
	set_En
	custom_delay(1);
	clear_En
}
void initialization_LCD()
{
	/* The module powers up in 8-bit mode.
	   The initial start-up instructions are sent in 8-bit mode, 
	   with the lower four bits (which are not connected) of each instruction
	    as don't cares */
	LCD_send_cmd(0x02);	// initialization in 4-bit mode
	
	
	/* setting 4bit communication and 2-line display and 5x8 dots format display */
	// Function Set --> D[7:4] -- 001(DL)_(N)(F)xx
	// DL === 0 -- 4bit mode
	// DL === 1 -- 8bit mode
	// N === 0 -- 1-line display
	// N === 1 -- 2-line display
	// F === 0 -- 5x8 dots format display mode
	// F === 1 -- 5x11 dots format display mode
	// so we select D[7:0] = 0b0010_10xx --- 0x28
	LCD_send_cmd(0x28);
	
	
	/* Setting up of cursor left movement on increment and display not shifting */
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0110 --- 0x06
	LCD_send_cmd(0x06);
		
	/* Setting up of disply on, cursor on and blink on	*/
	/* Display ON/OFF, Cursor ON/OFF  Blink ON/OFF*/
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on	
	// so we slect D[7:0] = 0b0000_1111 = 0x0F;
	LCD_send_cmd(0x0E);
	

	/* Set DDRAM address */
	// when 1- line display (N=0), DDRAM address from 00H to 4FH -- TOTAL 40
	// when 2- line display (N=1), DDRAM address from 00H to 27H for first line AND 40H to 67H for seoncd ine -- totall 20 + 20 = 40
	// D[7:0] --- 0b1(AC6)(AC5)(AC4)(AC3)(AC2)(AC1)(AC0)	
	// So we select D[7:0] = 0b1000_0000 = 0x80;
	
	LCD_send_cmd(0x80);
}
void LCD_clear()
{	
	// 0x01 for clearing display
	LCD_send_cmd(0x01);	
}
void LCD_home()
{
	// 0000_001x for moving cursor to inital position and moving DRAM address to 00H
	LCD_send_cmd(0x02);
}
void LCD_print_left()
{
	// moving left
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x04
	LCD_send_cmd(0x04);
}
void LCD_print_right()
{
	// moving left
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x06
	LCD_send_cmd(0x06);
}
void LCD_shift_left()
{
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x06
	LCD_send_cmd(0x07);
}
void LCD_shift_right()
{
	/* Works when there is character before the first character */
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x06
	LCD_send_cmd(0x05);
}
void LCD_display_off()
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_10xx = 0x0F;
	LCD_send_cmd(0x08);
}
void LCD_display_on_cursor_off_blink_off()
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1100 = 0x0E;
	LCD_send_cmd(0x0C);
}
void LCD_display_on_cursor_on_blink_off()
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1110 = 0x0E;
	LCD_send_cmd(0xE);
}
void LCD_display_on_cursor_off_blink_on()
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1101 = 0x0D;
	LCD_send_cmd(0x0D);
}
void LCD_display_on_cursor_on_blink_on()
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1111 = 0x0F;
	LCD_send_cmd(0x0F);
}
void LCD_cursor_leftShift()
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- 
	// (S/C)(R/L) === 10 -- 
	// (S/C)(R/L) === 11 -- 
	// so we slect D[7:0] = 0b0001_00xx = 0x10;
	LCD_send_cmd(0x010);	
}
void LCD_cursor_rightShift()
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- Shift Cursor to Right
	// (S/C)(R/L) === 10 --
	// (S/C)(R/L) === 11 --
	// so we slect D[7:0] = 0b0001_01xx = 0x14;
	LCD_send_cmd(0x014);	
}
void LCD_display_leftShift()
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- Shift Cursor to Right
	// (S/C)(R/L) === 10 -- Shift Display to Left
	// (S/C)(R/L) === 11 --
	// so we slect D[7:0] = 0b0001_10xx = 0x18;
	LCD_send_cmd(0x018);
}
void LCD_display_rightShift()
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- Shift Cursor to Right
	// (S/C)(R/L) === 10 -- Shift Display to Left
	// (S/C)(R/L) === 11 -- Shift Display to Left
	// so we slect D[7:0] = 0b0001_11xx = 0x1C;
	LCD_send_cmd(0x01C);
}
void LCD_send_string(char *c_string)
{
	while(*c_string != '\0')
	{
		LCD_send_data(*c_string++);
	}
}
void LCD_set_cursor(uint8_t r,uint8_t c)
{
	// r and c starts from 1
	if (r==1)
	{
		LCD_send_cmd(0x80+c-1);
	}
	else if (r==2)
	{
		LCD_send_cmd(0xC0+c-1);
	}
}

void send_string_l(uint8_t *c_string,uint8_t len)
{
	for (uint8_t i=0;i<len;i++)
	{
		LCD_send_data(c_string[i]);
	}
}

void LCD_print_num(int num,uint8_t dig)
{

	uint8_t buf[dig];

	itoa(num, buf, 10);
	
	send_string_l(buf,dig);
}
void LCD_init()
{
	port_init_LCD();
	initialization_LCD();
}

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
