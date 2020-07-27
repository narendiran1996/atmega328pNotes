#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>
uint8_t status = 0;
void I2C_Master_Init()
{
	// Intialize the I2C clock frequency to 100kHz
	// let the prescalr be 1
	// f_i2c =  F_CPU / (16 + (2*xTWBR*Prescaler)) = 32
	// setting the TWBR register.
	TWBR = 32;

	// writing 1 to prscalre
	// setting the TWPS bits in TWSR to 00
	TWSR &= ~(1<<TWPS0);
	TWSR &= ~(1<<TWPS1);
}
uint8_t I2C_Master_Status()
{
	// Status value are available from TWSR[7:3]
	return TWSR & 0XF8;
}
uint8_t I2C_Master_START()
{
	// Enabling the TWI interface
	TWCR |= (1<<TWEN);
	// sending START condition
	TWCR |= (1<<TWSTA);
	// Do the transaction
	TWCR |= (1<<TWINT);
	// Checking if START condition is sent correctly
	while((TWCR & (1<<TWINT )) == 0x00);
	status = I2C_Master_Status();
	// checking status if START condition is sent correctily
	if(status == 0x08)
	{
		// no error occured
		return 0;
	}
	else
	{
		// error occured
		return 0;
	}
}
uint8_t I2C_Master_STOP()
{
	// Removing Start condition on bit
	TWCR &= ~(1<<TWSTA);
	// sending STOP condition
	TWCR |= (1<<TWSTO);
	
	// Do the transaction
	TWCR |= (1<<TWINT);

	// disaabling stop and interface
	
	
	TWCR &= ~(1<<TWSTO);
	TWCR &= ~(1<<TWEN);

	return 0;
}
uint8_t I2C_Master_Mode(uint8_t slave_address, uint8_t transmiter0_receiver1)
{
	// Entering MASTER mode
	// Writing SLA+W into TWDR for transmiiter and SLA+R for receiver
	// slave address must be MSB first
	// slave address is left shifted by 1 in order to accompany the R/W bit
	TWDR = (slave_address<<1) | transmiter0_receiver1;
	// Do the transaction
	TWCR |= (1<<TWINT);
	while((TWCR & (1<<TWINT )) == 0x00);
	status = I2C_Master_Status();
	// For transmitter the staus would have to be 0x18 and for receiver 0x40
	uint8_t status_val_checker = (transmiter0_receiver1==0) ? 0x18 : 0x40;
	if(status == status_val_checker)
	{
		// no error occured
		return 0;
	}
	else
	{
		// error occured
		return 0;
	}
}
uint8_t I2C_Master_DataTransmitByte(uint8_t data_)
{
	// Data packet is transmitted
	// Writing data intor TWDR
	TWDR = data_;
	// Do the transaction
	TWCR |= (1<<TWINT);
	while((TWCR & (1<<TWINT )) == 0x00);
	status = I2C_Master_Status();
	if(status == 0x28)
	{
		// ACK received and still data can be sent
		return 0;
	}
	else if(status == 0x30)
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
void I2C_Master_DataTransmitString(uint8_t *cdata)
{
	while(*cdata != '\0')
	{
		status = I2C_Master_DataTransmitByte(*cdata++) ;
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

uint8_t I2C_Master_DataReceiveByte()
{
	uint8_t value_ = 0;

	// Data packet is recieved
	TWCR |= (1<<TWINT);
	// Do the transaction
	while((TWCR & (1<<TWINT )) == 0x00)
	{
		value_ = TWDR;
	}
	
	 	
	status = I2C_Master_Status();
	if(status == 0x58)
	{
		// no error occured
		return value_;
	}
	else
	{
		// error occured
		return 1;
	}
}
void I2C_Master_DataReceiveString(uint8_t *recData,uint8_t NUMBYTE)
{
	uint8_t i=0;
	recData[NUMBYTE] = '\0';
	while(i < NUMBYTE)
	{
		// Enabling the Acknowledment bit for replying positive ACK
		TWCR |= (1<<TWEA);
		if(i==(NUMBYTE-1))
		{
			// disbale the Acknowledment bit for replying Negatice ACK for last byte
			TWCR &= ~(1<<TWEA);
		}
		status = I2C_Master_DataReceiveByte();
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
	I2C_Master_Init();
	DDRC |= (1<<0) | (1<<1) | (1<<2);
	PORTC |= (1<<0) | (1<<1) | (1<<2);


	I2C_Master_START();
	I2C_Master_Mode(SLAVE_ADDRESS, 0);
	I2C_Master_DataTransmitString("K");
	I2C_Master_STOP();

	I2C_Master_START();
	I2C_Master_Mode(SLAVE_ADDRESS, 0);
	I2C_Master_DataTransmitString("Narendiran");
	I2C_Master_STOP();
	
	// uint8_t recData[15];
	// I2C_Master_START();	
	// I2C_Master_Mode(SLAVE_ADDRESS, 1);
	// I2C_Master_DataReceiveString(recData, 3);
	// I2C_Master_STOP();

	PINC |= (1<<2);
    while(1)
    {

    }
}
