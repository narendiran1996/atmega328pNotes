
// Reset -- arduino pin 8
#define RES PORTB0
// Data/Command -- arduino pin 9
#define DC PORTB1
// Chip Enable -- arduino pin 10
#define SS PORTB2
// Data In -- arduino pin 11
#define SIN PORTB3
// Serial Clock -- arduino pin 13
#define SCK PORTB5

#define chip_enable PORTB &= ~(1<<SS)
#define chip_disable PORTB |= (1<<SS)
#define cmd_mode PORTB &= ~(1<<DC)
#define data_mode PORTB |= (1<<DC)


void init_pins()
{
  DDRB |= (1<<RES) | (1<<DC) | (1<<SS) | (1<<SIN) | (1<<SCK);
  
  PORTB |= (1<<RES); // initially let's reset
  PORTB &= ~(1<<DC); // initially command mode
  PORTB |= (1<<SS); // initially disable chip
  PORTB &= ~(1<<SIN); // initially SIN is 0
  PORTB &= ~(1<<SCK); // initially SCK is 0
}
void init_spi()
{
  
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
  // we select Fosc/8 by 001
  SPSR &= ~(1<<SPI2X);
  SPCR &= ~(1<<SPR1);
  SPCR |= (1<<SPR0);
  
  // Enabling SPI
  SPCR |= (1<<SPE);
  
}
void SPI_transfer_byte(uint8_t val)
{
  SPDR = val;
  
  // wait till serial transmission is complete by checking the SPI Interrupt Flag
  while((SPSR & (1<<SPIF)) == 0 ) {};

  // return the recieved data - can use it or ignore it
  //return SPDR;
}

void rst_oled_init()
{
  chip_enable;
  PORTB |= (1<<RES);
  _delay_ms(10);
  PORTB &= ~(1<<RES);
  _delay_ms(10);
  PORTB |= (1<<RES);
}

void oled_send_command(uint8_t cmd_)
{
  cmd_mode;
  SPI_transfer_byte(cmd_);
}
void oled_send_data(uint8_t data_)
{
  data_mode;
  SPI_transfer_byte(data_);
}
void oled_init()
{
  
  init_pins();
  init_spi();
  rst_oled_init();

  // Set Display ON/OFF
  // D[7:0] == 0b1010_111(X0)
  // X0 = 0 - Display OFF
  // X0 = 1 - Display ON
  // we display off
  oled_send_command(0XAE);
  
  // Setting Clock divide Ratio and Oscillator Frequency - 0xD5
  oled_send_command(0XD5);
  // Setting 0b1000_0000 - 0x80
  // A[7:4]== 1000 for oscillator frequency for RESET
  // A[3:0]== 0000 for divide ratio of 1 for RESET
  oled_send_command(0x80);
  
  // Setting Multipler Ratio - 0xA8
  oled_send_command(0xA8);
  // any value of A[5:0] between 0b00_1111 to 0b11_1111
  // A[5:0]== 0b11_1111 for RESET
  oled_send_command(0x3f);
  
  
  // Set Display Offset - 0xD3
  oled_send_command(0xD3);
  // NO offset in display for RESET
  oled_send_command(0x00);
  
  
  // Set Display Start Line - 0X40
  oled_send_command(0x40 | 0);
  
  // Set Charge Pump - 0X8D
  oled_send_command(0x8D);
  // A[5:0] == 0b01_0(A2)00
  // A2 - 0 == Disable charge
  // A2 - 1 == Enable charge
  // we enable charge pump
  oled_send_command(0x14);
  
  // memory mode - 0x20
  oled_send_command(0x20);
  oled_send_command(0x00);                              // 0x0 act like ks0108
  
  // Set Segment Re-map --  0XA0 + disbale
  // dISBALING so that we get remapped address -- see diagram
  oled_send_command(0XA0 | 0x0);
  
  // Set COM Output Scan Direction
  // A[7:0] == 0b1100_(X3)000
  // X3 - 0 == Normal scan
  // X3 - 1 == Remapped Scan
  // We use normal scan
  oled_send_command(0xC0);
  
  // Set COM Pins Hardware Configuration - 0xDA
  oled_send_command(0xDA);
  // A[7:0] == 0b00(A5)(A4)_0010
  // A4 = 0 - Sequential COM pin configuration
  // A4 = 1 - RESET - alternative COM pin configuration
  // A5 = 0 - Disable COM Left/Rigth remap
  // A5 = 1 - Enable COM Left/Rigth remap
  // we use alternative COM pin configuration and disable COM left right rempa
  oled_send_command(0x12);
  
  // Set Contrast Control - 0x81
  oled_send_command(0x81);
  // any value between 1 and 256
  oled_send_command(0xAF);
  
  // Set pre-charge period - 0XD9
  oled_send_command(0xD9);
  // See data sheet -- 0xF1
  oled_send_command(0xF1);
  
  // Set VCOMH Deselect Level - 0xDB
  oled_send_command(0xDB);
  // See data sheet -- 0x20
  oled_send_command(0x20);
  
  // Set Entier Display On/Off
  // A[7:0] == 0b1010_010(X0)
  // X0 = 0 - Display OFF
  // X0 = 1 - Display ON
  // we display offf
  oled_send_command(0xA4);
  
  // Selection between normal or inverse mode
  // A[7:0] == 0b1010_011(X0)
  // X0 = 0 - normal display
  // X0 = 1 - inverse display
  // we seect normal display
  oled_send_command(0xA6);
  
  // Set Display ON/OFF
  // D[7:0] == 0b1010_111(X0)
  // X0 = 0 - Display OFF
  // X0 = 1 - Display ON
  // we display ON
  oled_send_command(0xAF);
}

void oled_set_pixel_one_byte(uint8_t page_, uint8_t column_)
{
  // Set page address - 0x22
  oled_send_command(0x22);
  // set page start address
  oled_send_command(page_);
  // set column End Address
  oled_send_command(8 - 1);
  
  // Set Column address - 0x21
  oled_send_command(0X21);
  // Set column  Start Address
  oled_send_command(column_);
  // Set column End address
  oled_send_command(128 - 1);
}


void oled_demo()
{
  
  oled_set_pixel_one_byte(0,0);
  oled_send_data(0b11111111);
  oled_set_pixel_one_byte(0,2);
  oled_send_data(0b11100111);
  oled_set_pixel_one_byte(0,4);
  oled_send_data(0b11110000);
  oled_set_pixel_one_byte(0,6);
  oled_send_data(0b00001111);
  oled_set_pixel_one_byte(0,8);
  oled_send_data(0b11001100);
  oled_set_pixel_one_byte(0,10);
  oled_send_data(0b10010010);
  
  for (uint8_t i=0; i<128; i++)
  {
    oled_set_pixel_one_byte(1,0+i);
    oled_send_data(0b10000001);
  }
  for (uint8_t i=0;i<8;i++)
  {
    oled_set_pixel_one_byte(0+i,12);
    oled_send_data(0b11111111);
  }
  uint8_t heartine[]={ 0x18, 0x3c, 0x7c, 0x3c, 0x18};
  for(uint8_t i=0;i<5;i++)
  {
    oled_set_pixel_one_byte(3,42+i);
    oled_send_data(heartine[i]);
  }
}
void oled_clear()
{
   
   for(uint8_t j=0;j<8;j++)
   {
    oled_set_pixel_one_byte(j,0);
     for(uint8_t i=0;i<128;i++)
     {
      oled_send_data(0x00);
     }
   }
}
void oled_uyir()
{
  // obtained by  GLCD Font creater from MikroElektronika
  uint8_t test16x16[] = {
    0x00, 0x1C, 0x00, 0x16, 0x00, 0x22, 0x38, 0x22, 0x7C, 0x22, 0x64, 0x22, 0x7C, 0x22, 0x3C, 0x22, 0x04, 0x22, 0x0C, 0x32, 0x08, 0x1A, 0x38, 0x0E, 0xE0, 0x07, 0x00, 0x03, 0xFC, 0x3F, 0x00, 0x00,  // Code for char 
    0x00, 0x00, 0x80, 0x01, 0xCC, 0x03, 0x5E, 0x02, 0x52, 0x02, 0x5E, 0x02, 0x42, 0x02, 0x46, 0x72, 0x5C, 0x5B, 0xF8, 0x59, 0x40, 0x70, 0xFE, 0x43, 0x40, 0x60, 0xC0, 0x38, 0x80, 0x0F, 0x00, 0x00,  // Code for char 
    0x00, 0x0E, 0x00, 0x1A, 0xF0, 0x13, 0x38, 0x16, 0x0C, 0x1A, 0x66, 0x1A, 0x93, 0x32, 0xF1, 0x52, 0x11, 0x52, 0x31, 0x52, 0xE1, 0x52, 0xC3, 0x5A, 0x86, 0x4F, 0x3C, 0x22, 0x60, 0x3E, 0x00, 0x00,  // Code for char 
    0x00, 0x00, 0xFE, 0x3F, 0xFE, 0x3F, 0x02, 0x00, 0x02, 0x00, 0xC2, 0x00, 0x02, 0x00, 0x02, 0x00, 0xFE, 0x3F, 0xFE, 0x3F, 0x02, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 
    0x00, 0x00, 0x00, 0x18, 0x78, 0x3C, 0x4C, 0x24, 0x4C, 0x24, 0x7C, 0x24, 0x08, 0x24, 0x78, 0x26, 0xE0, 0x23, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x00,  // Code for char 
    0x00, 0x18, 0x78, 0x3C, 0x4C, 0x24, 0x7C, 0x24, 0x0C, 0x26, 0x38, 0x27, 0xF0, 0x21, 0x00, 0x28, 0x00, 0x34, 0x00, 0x2A, 0x00, 0x22, 0x00, 0x3E, 0x00, 0x22, 0x00, 0x3E, 0x00, 0x22, 0x00, 0x00,  // Code for char 
    0x00, 0x00, 0x80, 0x0F, 0xC0, 0x11, 0x60, 0x11, 0x20, 0x1B, 0x10, 0x0E, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0xF0, 0x1F, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 
    0x00, 0x00, 0xF0, 0x01, 0x38, 0x02, 0x2C, 0x02, 0x64, 0x03, 0xC2, 0x41, 0x02, 0x60, 0x02, 0x30, 0x02, 0x10, 0x02, 0x08, 0x02, 0x0C, 0xFE, 0x07, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 
    0x00, 0x00, 0x18, 0x00, 0x64, 0x3E, 0x44, 0x71, 0x46, 0x41, 0x3E, 0x41, 0x02, 0x41, 0x0E, 0x61, 0x3E, 0x31, 0x02, 0x39, 0x02, 0x61, 0x02, 0x41, 0x06, 0x41, 0xFC, 0x41, 0x00, 0x7C, 0x00, 0x00,  // Code for char
    0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x9C, 0x00, 0x96, 0x1E, 0x62, 0x38, 0x03, 0x60, 0x03, 0x40, 0xC1, 0x40, 0x81, 0x41, 0x83, 0x63, 0xCE, 0x3E, 0x7C, 0x06, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00,  // Code for char
    0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x9C, 0x00, 0x96, 0x1E, 0x62, 0x32, 0x03, 0x7E, 0x03, 0x40, 0xC1, 0x40, 0x81, 0x41, 0x83, 0x63, 0xCE, 0x3E, 0x7C, 0x06, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00,  // Code for char
    0xF8, 0x00, 0x9C, 0x1C, 0xF4, 0x34, 0x04, 0x64, 0x04, 0x40, 0xC4, 0x40, 0x4C, 0x63, 0x78, 0x3E, 0x00, 0x00, 0x80, 0x03, 0x40, 0x00, 0xC0, 0x03, 0x40, 0x00, 0xC0, 0x03, 0x40, 0x00, 0x00, 0x00   // Code for char
  };
  int col_val=0;
  uint8_t page_inc=0;
  for (uint8_t letters=0;letters<12;letters++)
  {
    for (uint8_t i =0;i<16;i++)
    {
      uint8_t indx = i<<1;
      uint16_t let_sel = letters<<5;
      oled_set_pixel_one_byte(page_inc,col_val);
      oled_send_data(test16x16[let_sel + indx]);
      oled_set_pixel_one_byte(page_inc+1,col_val);
      oled_send_data(test16x16[let_sel + indx+1]);
      col_val++;
    }
    if ((col_val%128) ==0 )
    {
      page_inc=page_inc+2;
    }
  }
}

void setup() {
    oled_init();
  oled_clear();
  oled_uyir();

}

void loop() {
  // put your main code here, to run repeatedly:

}
