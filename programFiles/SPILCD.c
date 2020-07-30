#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Reset
#define RST PORTB0
// Data/Command
#define DC PORTB1
// Chip Enable
#define SS PORTB2
// Data In
#define SIN PORTB3
// Serial Clock
#define SCK PORTB5

#define chip_enable PORTB &= ~(1<<SS)
#define chip_disable PORTB |= (1<<SS)
#define cmd_mode PORTB &= ~(1<<DC)
#define data_mode PORTB |= (1<<DC)


void SPILCD_init_pins()
{
	DDRB |= (1<<RST) | (1<<DC) | (1<<SS) | (1<<SIN) | (1<<SCK);
	
	PORTB |= (1<<RST); // initially let's reset
	PORTB &= ~(1<<DC); // initially command mode
	PORTB |= (1<<SS); // initially disable chip	
	PORTB &= ~(1<<SIN); // initially SIN is 0
	PORTB &= ~(1<<SCK); // initially SCK is 0

}
void SPILCD_shiftOut(  uint8_t val)
{
	uint8_t i;

	for (i = 0; i<8; i++)  
	{
		if ((val & (1<<(7-i))) == 0)
		{
			PORTB &= ~(1<<SIN);
		}
		else
		{
			PORTB |= (1<<SIN);
		}
		
		PORTB |= (1<<SCK);
		PORTB &= ~(1<<SCK);
	}
}
void SPILCD_reset_procedure()
{
	PORTB |= (1<<RST); // initially let's reset
	
	chip_disable;
	PORTB &= ~(1<<RST); // remove reset
	_delay_ms(100);
	PORTB |= (1<<RST); // initially let's reset
}
void SPILCD_send_cmd(uint8_t cmd_)
{
	cmd_mode;
	
	chip_enable;
	
	SPILCD_shiftOut(cmd_);
	chip_disable;
}	
void SPILCD_send_data(uint8_t data_)
{
	data_mode;
	
	chip_enable;
	SPILCD_shiftOut(data_);
	chip_disable;
}
void SPILCD_init()
{
	SPILCD_init_pins();
	SPILCD_reset_procedure();

	
	/* Function Set -- DB[7:0] ---> 0010_0(PD)(V)(H)
	   PD = 1 - POWER DOWN mode
	   PD = 0 - chip is active mode
	   V = 0 - horizontal addressing
	   V = 1 - vertical addressing
	   H = 0 - Basic Instruction Set
	   H = 1 - Extended Instruction Set */
	// We use chip active, horizontal addressing and extended instruction Set
	// Extended because we need to set bias, temperature and OPERATING voltage
	// So DB[7:0] ---> 0010_0001 = 0X21
	SPILCD_send_cmd(0x21);
	/* SEtting Vop - lcd voltage - operating voltage 
	DB[7:0] ---> 1(Vop6)(Vop5)(Vop4)(Vop3)(Vop2)(Vop1)(Vop0)
	-- setting 5V we need == 100_0000 */
	// So DB[7:0] ---> 1100_0000 = 0XC0
	SPILCD_send_cmd(0xc0);
	
	/* Bias System -- DB[7:0] ---> 0001_0(BS2)(BS1)(BS0)
	   -- setting bias voltage level (n = 4, 1:48) --- 011
	   So DB[7:0] ---> 0001_0011 = 0X13 */
	SPILCD_send_cmd(0x13);
	
	/* Temperature Control  -- DB[7:0] ---> 0000_01(TC1)(TC0)
	   we take temparture coefficent 3 */
	// So PB[7:0] ---> 0000_0111 = 0X07
    SPILCD_send_cmd(0x07);
	
	/* Function Set -- DB[7:0] ---> 0010_0(PD)(V)(H)
	   PD = 1 - POWER DOWN mode
	   PD = 0 - chip is active mode
	   V = 0 - horizontal addressing
	   V = 1 - vertical addressing
	   H = 0 - Basic Instruction Set
	   H = 1 - Extended Instruction Set */
	// We use chip active, horizontal addressing and BASIC instruction Set
	// Basic because we are going to work now
	// So DB[7:0] ---> 0010_0000 = 0X20	
	SPILCD_send_cmd(0x20);
	
	/* Display Control -- DB[7:0] ---> 0000_1(D)0(E)
	   D | E ------- Mode ------- Description
	   0   0     Display Blank    No Display on LCD
	   0   1     Normal Mode      usual display on LCD
	   1   0     All Segment on   every position in the LCD is on
	   1   1     Inverse Mode     Display data is inverted */
	// So we select NOrmal mode
	// So DB[7:0] ---> 0000_1100 = 0x0c
	SPILCD_send_cmd(0x0C);
	
	/* setting X address varying pixel from 0 to 83[53H]
	   PB[7:0] ---> 1(X6)(X5)(X4)(X3)(X2)(X1)(X0)
	   X[6:0] is from 000_0000[00H] to 101_0011[53H] */
	// We select the middle column == 84/2 = 42[2A] 
	// So PB[7:0] ---> 1010_1010 = 0xAA
	SPILCD_send_cmd(0xAA);
	
	/* setting Y address varying blank from 0 to 5[5H]
	   PB[7:0] ---> 0100_0(Y2)(Y1)(Y0)
	   Y[2:0] is from 000[0H] to 101[5H] */
	// We select the middle row == 0
	// So PB[7:0] ---> 0100_0000 = 0x40
	SPILCD_send_cmd(0x40);
	/* Sending data to draw 
	   DB[7:0] ---> (D7)(D6)(D5)(D4)(D3)(D2)(D1)(D0) 
	   each bit represent each pixel in a blank */
	// So lets' draw something 
	SPILCD_send_data(0b10101010);
}
void SPILCD_pixel(uint8_t blank, uint8_t column)
{
	/* setting X address varying pixel from 0 to 83[53H]
	PB[7:0] ---> 1(X6)(X5)(X4)(X3)(X2)(X1)(X0)
	X[6:0] is from 000_0000[00H] to 101_0011[53H] */
	SPILCD_send_cmd(0x80 + column);
	
	/* setting Y address varying blank from 0 to 5[5H]
	PB[7:0] ---> 0100_0(Y2)(Y1)(Y0)
	Y[2:0] is from 000[0H] to 101[5H] */
	SPILCD_send_cmd(0x40 + blank);
}
void SPILCD_demo()
{
	SPILCD_pixel(0,0);
	SPILCD_send_data(0b11111111);	
	SPILCD_pixel(0,2);
	SPILCD_send_data(0b11111111);	
	SPILCD_pixel(0,4);
	SPILCD_send_data(0b11110000);	
	SPILCD_pixel(0,6);
	SPILCD_send_data(0b00001111);	
	SPILCD_pixel(0,8);
	SPILCD_send_data(0b11001100);	
	SPILCD_pixel(0,10);
	SPILCD_send_data(0b10010010);
	
	for (uint8_t i=0; i<84; i++)
	{
		SPILCD_pixel(2,0+i);
		SPILCD_send_data(0b00000001);
	}
	for (uint8_t i=0;i<6;i++)
	{
		SPILCD_pixel(0+i,12);
		SPILCD_send_data(0b11111111);
	}
	uint8_t heartine[]={ 0x18, 0x3c, 0x7c, 0x3c, 0x18};
	for(uint8_t i=0;i<5;i++)
	{
		SPILCD_pixel(3,42+i);
		SPILCD_send_data(heartine[i]);
	}	
}
int main(void)
{
	//lcd_init();
	//lcd_send_string("hi guys");
	SPILCD_init();
	SPILCD_demo();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}