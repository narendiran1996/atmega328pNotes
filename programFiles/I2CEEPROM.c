#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "UARTLCDinclude.c"
#include "I2CMASTERinclude.c"

// This address is 7bit without any shifting for R/W bit as my implmentation of I2C takes care of it
#define SLAVE_ADDRESS 0b1010000

void EEPROM_writeByte(uint8_t data_byte, uint8_t block_addres, uint8_t word_address)
{
	// start condition
	I2C_Master_START();
	
	// Enter Master Transmiter Mode -0
	I2C_Master_Mode(SLAVE_ADDRESS | block_addres ,0);
	
	// Accoding to datasheet of EEPROM, the word address iss ent
	I2C_Master_DataTransmitByte(word_address);
	
	// Next the actual data is sent
	I2C_Master_DataTransmitByte(data_byte);
	
	// Leave Master Tranmitter Mode -0 and stop condition
	I2C_Master_STOP();
}
void EEPROM_writeString(uint8_t *my_str, uint8_t block_address, uint8_t word_address)
{
	uint8_t string_count=0;
	// start condition
	I2C_Master_START();
	
	// Enter Master Transmiter Mode -0
	I2C_Master_Mode(SLAVE_ADDRESS | block_address ,0);
	
	// Accoding to datasheet of EEPROM, the word address iss ent
	I2C_Master_DataTransmitByte(word_address);
	
	while(*my_str != '\0')
	{
		string_count++;
		if (string_count==17)
		{
			// if more than sixteen we change the page	
		
			string_count = 0;
			I2C_Master_STOP();
			
			I2C_Master_START();
			
			I2C_Master_Mode(SLAVE_ADDRESS | block_address ,0);
			
			I2C_Master_DataTransmitByte(word_address + 16);
			
		}
		I2C_Master_DataTransmitByte(*my_str++);
	}
	// Leave Master Tranmitter Mode -0 and stop condition
	I2C_Master_STOP();
	
}
uint8_t EEPROM_readByte(uint8_t block_address, uint8_t word_address)
{
	// start condition
	I2C_Master_START();
	
	// For reading first word address needs to be sent  so we enter master transmitter
	// Enter Master Transmiter Mode -0
	I2C_Master_Mode(SLAVE_ADDRESS | block_address,0);
	
	I2C_Master_DataTransmitByte(word_address);
	I2C_Master_STOP();
	
	//Next, we can read
	I2C_Master_START();
	// Enter Master Receiver Mode -0
	I2C_Master_Mode(SLAVE_ADDRESS | block_address,1);
	uint8_t value_ = I2C_Master_DataReceiveByte();	
	I2C_Master_STOP();
	
	return value_;
}
void EEPROM_read_string(uint8_t *redval,uint8_t lenght_,uint8_t block_address, uint8_t word_address)
{
	uint8_t i;
	for (i=0;i<lenght_;i++)
	{
		 redval[i] = EEPROM_readByte(block_address,word_address + i);	
	}
	redval[i] = '\0';
}


int main(void)
{
	UARTLCD_init();	
	I2C_Master_Init();
	
	// The device addressing is 101_0(B2,B1,B0)
	// B[2:0] -- Block Select bits
	// Since the memory size of 24LC16B is as eight 256x8-bit memory
	// Each block has 256Bytes of data
	// So total of 8block x 256 Bytes = 2KByte	
	
	EEPROM_writeByte('A', 0, 0);
	UARTLCD_set_cursor(1,1);
	_delay_ms(10);
	char a=EEPROM_readByte(0,0);
	UARTLCD_send_data(a);
	
	EEPROM_writeString((uint8_t *)"abcdefghijklmnopqrstuvwxyz",0,0);
	_delay_ms(10);
	UARTLCD_set_cursor(2,1);
	uint8_t redval[50];
	EEPROM_read_string(redval,18,0,0);

	UARTLCD_send_string((char *)redval);
	while(1)
	{
		//TODO:: Please write your application code
	}
}