#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
uint8_t status = 0;
void I2C_SlaveInit(uint8_t my_address)
{
	// slave address  and last LSB 0 is for general call
	TWAR = (my_address<<1) & 0xFE;
	// Enabling the TWI interface.
	TWCR |= (1<<TWEN);
	// Disabling Start and Stop conditon bits
	TWCR &= ~(1<<TWSTA);
	TWCR &= ~(1<<TWSTO);

}
uint8_t I2C_Status()
{
	// Status value are available from TWSR[7:3]
	return TWSR & 0XF8;
}

uint8_t I2C_SlaveMode( uint8_t transmiter0_receiver1)
{
	// Acknowldege the address
	TWCR |= (1<<TWEA);
	// Watiting for the Master to call this slave
	while((TWCR & (1<<TWINT )) == 0x00);
	status = I2C_Status();
	// For transmitter the staus would have to be 0xA8 and for receiver 0x60
	uint8_t status_val_checker = (transmiter0_receiver1==0) ? 0xA8 : 0x60;
	if(status == status_val_checker)
	{
		// Master called this slave
		return 0;
	}
	else
	{
		// error occured
		return 1;
	}
}
uint8_t I2C_Slave_DataTransmitByte(uint8_t data_)
{
	// Data packet is transmitted
	// Writing data intor TWDR
	TWDR = data_;
	// Do the transaction
	TWCR |= (1<<TWINT);
	while((TWCR & (1<<TWINT )) == 0x00);

	status = I2C_Status();
	if(status == 0xB8)
	{
		// ACK received and still data can be sent
		return 0;
	}
	else if(status == 0xC8)
	{
		// NACK received and this is the last data so stop
		return 1;
	}
	else
	{
		// error occured
		return 2;
	}
}
void I2C_Slave_DataTransmitString(char *cdata)
{
	uint8_t i = 0;
	while(cdata[i] != '\0')
	{
		status = I2C_Slave_DataTransmitByte(cdata[i]) ;
		i++;
		if(status == 0)
		{
			// ACK received and still data can be sent
			// continue
		}
		else if(status == 1)
		{
			// NACK received and this is the last data so stop
			return;
		}
		else
		{
			// error occured
			return;
		}
	}
}

uint8_t I2C_Slave_DataReceiveByte()
{
	uint8_t value_ = 0;

	// Data packet is recieved
	TWCR |= (1<<TWINT);
	// Do the transaction
	while((TWCR & (1<<TWINT )) == 0x00)
	{
		value_ = TWDR;
	}
	
	status = I2C_Status();
	if(status == 0x80)
	{
		// Data is sent and ACK has been returned
		return value_;
	}
	else if(status == 0x88)
	{
		// Data is sent and NACK has been returned for last byte
		return value_;
	}
	else
	{
		// error occured
		return 0xFF;
	}
}
void I2C_Slave_DataReceiveString(uint8_t *recData,uint8_t NUMBYTE)
{
	uint8_t i=0;
	recData[NUMBYTE] = '\0';
	while(NUMBYTE > 0)
	{
		NUMBYTE = NUMBYTE - 1;
		// Enabling the Acknowledment bit for replying positive ACK
		TWCR |= (1<<TWEA);
		if(NUMBYTE==0)
		{
			// disbale the Acknowledment bit for replying Negatice ACK for last byte
			TWCR &= ~(1<<TWEA);
		}
		status = I2C_Slave_DataReceiveByte();
		if(status==0xFF)
			return;
		else		
			recData[i] = status;
		i++;
	}
}
#define SLAVE_ADDRESS 0b01010101


int main(void)
{
	DDRC |= (1<<0) | (1<<1) | (1<<2);
	PORTC |= (1<<0) | (1<<1) | (1<<2);


	// I2C_SlaveInit(SLAVE_ADDRESS);
	// I2C_SlaveMode(1);
	// uint8_t recData[11];
	// I2C_Slave_DataReceiveString(recData, 1);
	// TWCR &= ~(1<<TWEN);
	// if(recData[0]=='K')
	// 	PINC |= (1<<0);

	// I2C_SlaveInit(SLAVE_ADDRESS);
	// I2C_SlaveMode(1);
	// I2C_Slave_DataReceiveString(recData, 5);
	// TWCR &= ~(1<<TWEN);
	// if(strcmp(recData, "Naren")==0)
	// 	PINC |= (1<<1);

	char *abcd ="AaBbCa";
	I2C_SlaveInit(SLAVE_ADDRESS);
	I2C_SlaveMode(0);
	for(int j=0;j<4;j++)
	I2C_Slave_DataTransmitByte(abcd[j]);
	TWCR &= ~(1<<TWEN);
	PINC |= (1<<2);
    while(1)
    {

    }
}
