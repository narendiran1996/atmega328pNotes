#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>

void USART0init()
{
    // Setting up the Mode
    // Select the Asyncronous Master Mode.
    // Setting UMSEL0[1:0] in UCSR0C to 00
    UCSR0C &= ~(1<<UMSEL00);
    UCSR0C &= ~(1<<UMSEL01);

    // setting up the Buad rate
    // Due to The Clock rate being 8MHz, for a buad rate of 9600
    // UBRR0 = (fosc / (16*BAUD)) -1
    // So UBRR0 = (8000000 / (16 * 9600)) - 1 = 0x33
    UBRR0H = 0x00;
	UBRR0L = 0x33;

    // setting up the Frame Format
    // Let's select 8-bit data bits, no parity, and 1 stop bit
    // 8 - bit data bits
    // By selecting UCSZ0[2:0] in UCSR0C and UCSR0B register to be 011
	UCSR0B &= ~(1<<UCSZ02);
	UCSR0C |= (1<<UCSZ01);
	UCSR0C |= (1<<UCSZ00);
    // No parity
    // By selecting UPM0[1:0] in UCSR0C to 00
    UCSR0C &= ~(1<<UPM01);
    UCSR0C &= ~(1<<UPM00);
    // 1 stop bit
    // By selecting USBS0 in UCSR0C to 0 
    UCSR0C &= ~(1<<USBS0);

    // Disabling any interrupts
    UCSR0B &= ~(1<<7);
    UCSR0B &= ~(1<<6);
    UCSR0B &= ~(1<<5);

    // Enabling Transmitter 
    UCSR0B |= (1<<TXEN0);
    // Enabling Receiver
    UCSR0B |= (1<<RXEN0);

}
void USART0sendChar(uint8_t data_)
{
	//cHECKING if transmitet buffer is empty
	while((UCSR0A & (1<<UDRE0)) == 0x00){};		
	UDR0 = data_;	
}
void USART0sendString(uint8_t *c_data_)
{
	while(*c_data_ != '\0')
	{
		USART0sendChar(*c_data_++);
	}
}
uint8_t USART0receiveChar()
{
	// wait for thedate to be recied
	while((UCSR0A & (1<<RXC0)) == 0x00){};		
	return UDR0;
}
uint8_t *USART0receiverStringUntil(uint8_t deliminator)
{
	uint16_t i=0;
	uint8_t rec_buff[1024];
	uint8_t curr_char = USART0receiveChar();
	while(curr_char != deliminator)
	{
		rec_buff[i] = curr_char;
		curr_char = USART0receiveChar();
		i++;
	}
	rec_buff[i] = '\0';
	return rec_buff;
}
int main(void)
{
	USART0init();
    USART0sendString("This is working\n\r");
    while(1)
    {
        uint8_t *a = usart0_receive_string_till('\n');
        _delay_ms(100);
    }
}
