#ifndef LCDinclude_h_
#define LCDinclude_h_

// for iota function
#include <stdlib.h>

// connect PC0 TO EN
// connect PC1 TO RS
// connect PC[5:2] TO LCD[7:4]
#define set_En PORTC |= (1<<PORTC0);
#define clear_En PORTC &= ~(1<<PORTC0);

#define set_RS PORTC |= (1<<PORTC1);
#define clear_RS PORTC &= ~(1<<PORTC1);



void LCD_set_MSB(uint8_t val_);
void LCD_set_LSB(uint8_t val_);
void LCD_custom_delay(uint8_t del_val);
void LCD_port_init(void);
void LCD_send_cmd(uint8_t cmd_);
void LCD_send_data(uint8_t data_);
void LCD_sequence(void);
void LCD_clear(void);
void LCD_home(void);
void LCD_print_left(void);
void LCD_print_right(void);
void LCD_shift_left(void);
void LCD_shift_right(void);
void LCD_display_off(void);
void LCD_display_on_cursor_off_blink_off(void);
void LCD_display_on_cursor_on_blink_off(void);
void LCD_cursor_leftShift(void);
void LCD_cursor_rightShift(void);
void LCD_display_rightShift(void);
void LCD_send_string(char *c_string);
void LCD_set_cursor(uint8_t r,uint8_t c);
void LCD_send_string_l(uint8_t *c_string,uint8_t len);
void LCD_print_num(int num,uint8_t dig);
void LCD_init(void);


#endif