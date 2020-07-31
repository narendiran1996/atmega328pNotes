#ifndef I2CMASTERinclude_h_
#define I2CMASTERinclude_h_
uint8_t status;
void I2C_Master_Init(void);
uint8_t I2C_Master_Status(void);
uint8_t I2C_Master_START(void);
uint8_t I2C_Master_STOP(void);
uint8_t I2C_Master_Mode(uint8_t slave_address, uint8_t transmiter0_receiver1);
uint8_t I2C_Master_DataTransmitByte(uint8_t data_);
void I2C_Master_DataTransmitString(uint8_t *cdata);
uint8_t I2C_Master_DataReceiveByte(void);
void I2C_Master_DataReceiveString(uint8_t *recData,uint8_t NUMBYTE);

#endif