#define F_CPU 16000000L


#include <avr/io.h>
#include <util/delay.h>


#include "I2CMASTERinclude.c"

#define SLAVE_ADDRESS 0x3E // obtained by reversing the I2C address in PROTESS I2C lcd address

void I2CLCD_send_cmd(uint8_t cmd_)
{
	// start condtion
	I2C_Master_START();
	 _delay_ms(10);
	// addressing the slave
	I2C_Master_Mode(SLAVE_ADDRESS, 0);
	 _delay_ms(10);
	// sending command mode
	I2C_Master_DataTransmitByte(0x00);
	 _delay_ms(10);
	// Sending actual command
	I2C_Master_DataTransmitByte(cmd_);
	 _delay_ms(10);
	I2C_Master_STOP();	
    _delay_ms(10);
}
void I2CLCD_send_data(uint8_t data_)
{
	// start condtion
	I2C_Master_START();
	 _delay_ms(10);
	// addressing the slave
	I2C_Master_Mode(SLAVE_ADDRESS, 0);
	 _delay_ms(10);
	// sending data mode
	I2C_Master_DataTransmitByte(0x40);
	 _delay_ms(10);
	// Sending actual data
	I2C_Master_DataTransmitByte(data_);
	 _delay_ms(10);
	I2C_Master_STOP();
    _delay_ms(10);
}
void I2CLCD_init()
{
	I2C_Master_Init();	
	I2CLCD_send_cmd(0x38);	
	I2CLCD_send_cmd(0x06); // cursoR left moment and display not shift
	I2CLCD_send_cmd(0x0E); // dsiply on cursor on blink on
	I2CLCD_send_data(0x80); // BGIGIND OF libne
}
int main(void)
{
	I2CLCD_init();
	I2CLCD_send_data('a');
    
	I2CLCD_send_data('v');
    while(1)
    {
        //TODO:: Please write your application code 
    }
}