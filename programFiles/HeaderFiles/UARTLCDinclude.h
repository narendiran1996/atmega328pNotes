#ifndef UARTLCDinclude_h_
#define UARTLCDinclude_h_

// for iota function
#include <stdlib.h>
// for delay
#include <util/delay.h>

void UARTLCD_init(void);
void UARTLCD_send_data(uint8_t data_);
void UARTLCD_send_cmd(uint8_t cmd_);
void UARTLCD_clear(void);
void UARTLCD_home(void);
void UARTLCD_print_left(void);
void UARTLCD_print_right(void);
void UARTLCD_shift_left(void);
void UARTLCD_shift_right(void);
void UARTLCD_display_off(void);
void UARTLCD_display_on_cursor_off_blink_off(void);
void UARTLCD_display_on_cursor_on_blink_off(void);
void UARTLCD_display_on_cursor_off_blink_on(void);
void UARTLCD_display_on_cursor_on_blink_on(void);
void UARTLCD_cursor_leftShift(void);
void UARTLCD_cursor_rightShift(void);
void UARTLCD_display_leftShift(void);
void UARTLCD_display_rightShift(void);
void UARTLCD_send_string(char *c_string);
void UARTLCD_set_cursor(uint8_t r,uint8_t c);
void UARTLCD_send_string_l(uint8_t *c_string,uint8_t len);
void UARTLCD_print_num(int num,uint8_t dig);



#endif