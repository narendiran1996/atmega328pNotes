/*
en -- Q7
rs -- Q6
d4 -- Q5
d5 -- Q4
d6 -- Q3
d7 -- Q2
*/
//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;
int oe = 9;
uint8_t sh_val_;
void init_sh()
{
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(oe,OUTPUT);
  digitalWrite(oe, 0);
  
  sh_val_=0;
  write_sh_data();
}
void write_sh_data()
{
  digitalWrite(latchPin, LOW);
  // shift out the bits:
  shiftOut(dataPin, clockPin, MSBFIRST, sh_val_);  
  
  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
}

static const uint8_t BitReverseTable256[256] = 
{
#   define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
#   define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
#   define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )
    R6(0), R6(2), R6(1), R6(3)
};

#define set_En {\
                sh_val_ = sh_val_ | (1<<7);\
                write_sh_data();\
                }
#define clear_En {\
                sh_val_ = sh_val_ & ~(1<<7);\
                write_sh_data();\
                }

#define set_RS {\
                sh_val_ = sh_val_ | (1<<6);\
                write_sh_data();\
                }
#define clear_RS {\
                sh_val_ = sh_val_ & ~(1<<6);\
                write_sh_data();\
                }
#define set_LSB(val_) {\
                     sh_val_ = ((sh_val_ & 0b11000011) | ((BitReverseTable256[val_] & 0xF0)>>2));\
                      write_sh_data();\
                      }      
#define set_MSB(val_) {\
                      sh_val_ = ((sh_val_ & 0b11000011) | ((BitReverseTable256[val_] & 0x0F)<<2));\
                      write_sh_data();\
                      }  

void custom_delay(uint8_t del_val)
{
  delay(10);
}

void lcd_send_cmd(uint8_t cmd_)
{
  set_MSB(cmd_)
  clear_RS
  set_En
  custom_delay(1);
  clear_En

  set_LSB(cmd_)
  clear_RS
  set_En
  custom_delay(1);
  clear_En

  delay(10);
}

void lcd_send_data(uint8_t data_)
{
  set_MSB(data_)
  set_RS
  set_En
  custom_delay(1);
  clear_En

  set_LSB(data_)
  set_RS
  set_En
  custom_delay(1);
  clear_En
  delay(10);
}

void initialization_lcd()
{
  /* The module powers up in 8-bit mode.
     The initial start-up instructions are sent in 8-bit mode, 
     with the lower four bits (which are not connected) of each instruction
      as don't cares */
  lcd_send_cmd(0x02); // initialization in 4-bit mode
  
  /* setting 4bit communication and 2-line display and 5x8 dots format display */
  // Function Set --> D[7:4] -- 001(DL)
  // DL === 0 -- 4bit mode
  // DL === 1 -- 8bit mode
  // D[3:0] -- (N)(F)xx
  // N === 0 -- 1-line display
  // N === 1 -- 2-line display
  // F === 0 -- 5x8 dots format display mode
  // F === 1 -- 5x11 dots format display mode
  // so we select D[7:0] = 0b0010_10xx --- 0x28
  lcd_send_cmd(0x28);
  
  /* Setting up of cursor left movement and display not shifting */
  /* Entry Mode */
  // Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
  // I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
  // I/D === 1 -- Cursor/Blink moves to righ and DDRAM address is decreased by 1
  // S === 0 -- Dont' Shifht the entire display
  // S === 1 -- Shift the entire display
  // so we select D[7:0] = 0b0000_0110 --- 0x06
  lcd_send_cmd(0x06);
    
  /* Setting up of disply on, cursor on and blink on  
  /* Display ON/OFF, Cursor ON/OFF  Blink ON/OFF*/
  // Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
  // D === 0 -- display off display data is remained in DDRAM
  // D === 1 -- display on
  // C === 0 -- cursor is disabled in current display, but I/D register data is reamins
  // C === 1 -- cursto is turned on
  // B === 0 -- Blink off
  // B === 1 -- Blink on  
  // so we slect D[7:0] = 0b0000_1111 = 0x0F;
  lcd_send_cmd(0x0F);
  

  /* Set DDRAM address */
  // when 1- line display (N=0), DDRAM address from 00H to 4FH -- TOTAL 40
  // when 2- line display (N=1), DDRAM address from 00H to 27H for first line AND 40H to 67H for seoncd ine -- totall 20 + 20 = 40
  // D[7:0] --- 0b1(AC6)(AC5)(AC4)(AC3)(AC2)(AC1)(AC0)  
  // So we select D[7:0] = 0b1000_0000 = 0x80;
  
  lcd_send_cmd(0x80);



  /* for clearing it -- for 74595 usage */

  lcd_set_cursor(1,1); 
  lcd_send_string("----------------");
  lcd_set_cursor(2,1);
  lcd_send_string("----------------");
  delay(10);
  lcd_set_cursor(1,1); 
  lcd_send_string("                ");
  lcd_set_cursor(2,1);
  lcd_send_string("                ");
  delay(10);

  lcd_send_cmd(0x01);
  delay(100);
}
void lcd_set_cursor(uint8_t r,uint8_t c)
{
  if (r==1)
  {
    lcd_send_cmd(0x80+c-1);
  }
  else
  {
    lcd_send_cmd(0xC0+c-1);
  }
}
void lcd_send_string(uint8_t *c_string)
{ 
  while(*c_string != '\0')
  {
    lcd_send_data(*c_string++);
  }
}
void setup()
{
  init_sh();
  initialization_lcd();
   lcd_set_cursor(1,1); 
   lcd_send_string("is there prob still?");

}

void loop()
{

}
