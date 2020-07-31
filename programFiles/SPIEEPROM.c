#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "UARTLCDinclude.c"
#define ss_low PORTB &= ~(1<<PORTB2);
#define ss_high PORTB |= (1<<PORTB2);

void SPI_init()
{
	// making SCK, MOSI, SS' as outptut
	DDRB |= (1<<DDB2) | (1<<DDB3) | (1<<DDB5);
	// making MISO as input
	DDRB &= ~(1<<DDB4);

	// making SCK, MOSI,  as low
	PORTB &=  ~(1<<PORTB3) & ~(1<<PORTB5);
	// making SS' as high
	ss_high
	
	
	// dISBALE SPIE bit for interrupt on Serial Transfer Completion
	SPCR &= ~(1<<SPIE);
	
	// Select MSB first or LSB first by DORD
	SPCR &= ~(1<<DORD);
	
	// Select this as Master
	SPCR |= (1<<MSTR);
	
	// Let the clock polarity be SCK is low when idle
	SPCR &= ~(1<<CPOL);
	
	// Sampled at Rising or Falling Edge
	// we choose rising edge
	SPCR &= ~(1<<CPHA);
	
	// Selecting a SCK frequnecy
	// we select Fosc/4 by 000
	SPSR &= ~(1<<SPI2X);
	SPCR &= ~(1<<SPR1);
	SPCR &= ~(1<<SPR0);
	
	// Enabling SPI
	SPCR |= (1<<SPE);
	
}
uint8_t SPI_transfer_byte(uint8_t data_)
{
	SPDR = data_;
	
	// wait till serial transmission is complete by checking the SPI Interrupt Flag
while((SPSR & (1<<SPIF)) == 0 ) {};

// return the recieved data - can use it or ignore it
return SPDR;
}


void EEPROM_write_string(uint16_t address_,uint8_t *c_data)
{
	ss_low
	SPI_transfer_byte(0X06); // WREN instruction -- according to EEPROM
	ss_high
	_delay_ms(1);

	
	ss_low
	SPI_transfer_byte(0X02); // WRITE instruction -- according to EEPROM
	// since  address is 16 bi
	SPI_transfer_byte(address_>>8); // sendiNG MSB data first
	SPI_transfer_byte(address_); // address where data should be writen
	
	while(*c_data != '\0')
	{
		SPI_transfer_byte(*c_data++); // data to be written
	}
	
	ss_high
}
void EEPROM_write_byte(uint16_t address_, uint8_t data_)
{	
	ss_low
	SPI_transfer_byte(0X06); // WREN instruction -- according to EEPROM
	ss_high
	
	_delay_ms(1);
	
	ss_low
	SPI_transfer_byte(0X02); // WRITE instruction -- according to EEPROM
	
	// since  address is 16 bi
	SPI_transfer_byte(address_>>8); // sendiNG MSB data first
	SPI_transfer_byte(address_); // address where data should be writen	
	
	SPI_transfer_byte(data_); // data to be written	
	ss_high	
	
	_delay_ms(1);
}
uint8_t EEPROM_read(uint16_t address_)
{
	uint8_t data_;
	ss_low
	SPI_transfer_byte(0x03); // READ instruction -- according to EEPROM
	
	SPI_transfer_byte(address_>>8); // sendiNG MSB data first
	SPI_transfer_byte(address_); // address where data should be writen
	
	data_=SPI_transfer_byte(0XFF); // send empty value to get data from it
	
	ss_high
	
	_delay_ms(1);
	return data_;
}
int main(void)
{
	UARTLCD_init();


	SPI_init();	
	_delay_ms(10);
	
	EEPROM_write_string(0x0000,(uint8_t *)"hi hello and welcome");	
	_delay_ms(10);
	for(uint8_t i=0; i<16; i++)
	{
		uint8_t val = EEPROM_read(0x00+i);
		UARTLCD_set_cursor(2,1+i);
		UARTLCD_send_data(val);
	}	
    while(1)
    {    }
}