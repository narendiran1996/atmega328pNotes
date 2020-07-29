#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
void SPI_Init()
{
    // making SCK, MOSI, SS' as outptut
    DDRB |= (1<<DDB2) | (1<<DDB3) | (1<<DDB5);
    // making MISO as input
    DDRB &= ~(1<<DDB4);
    // making SCK, MOSI, as low
    PORTB &= ~(1<<PORTB3) & ~(1<<PORTB5);
    // making SS' as high
    PORTB |= (1<<PORTB2);
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
    // dISBALE SPIE bit for interrupt on Serial Transfer Completion
    SPCR &= ~(1<<SPIE);
    // Enabling SPI
    SPCR |= (1<<SPE);
}
uint8_t SPITransferReceive(uint8_t data_)
{
    SPDR = data_;
    // wait till serial transmission is complete by checking the SPI Interrupt Flag
    while((SPSR & (1<<SPIF)) == 0 ) {};
    // return the recieved data - can use it or ignore it
    return SPDR;
}
int main(void)
{
    SPI_Init();
    PORTB &= ~(1<<PORTB2);
    SPITransferReceive('A');
    while(1)
    {

    }
}
