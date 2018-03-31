#include "main.h"

#define FCY (_XTAL_FREQ / 2)
#define I2C_BAUDRATE    (unsigned long)100000 //default baud rate 100kHz
#define I2C_ERROR           -1
#define I2C_OK              1

void I2C_Init(void);
int setBaudRate(void);
int I2C_WriteReg(char dev_addr, char reg_addr, char value);
int I2C_ReadReg(char dev_addr, char reg_addr, uint8_t *value);

void I2C2Write(uint8_t devAddr,uint16_t regAddr,uint8_t data);
void I2C2writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
bool I2C2writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
void I2C2writeBytes(uint8_t devAddr,uint8_t regAddr,uint8_t len,uint8_t *dptr);
void I2C2writeWord(uint8_t devAddr,uint8_t regAddr,uint16_t data);

uint8_t I2C2Read(uint8_t devAddr,uint16_t regAddr);
uint8_t I2C2readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data);
uint8_t I2C2readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
void I2C2readBytes(uint8_t devAddr,uint8_t regAddr,uint8_t len,uint8_t *dptr);

unsigned int StartI2C(void);
unsigned int RestartI2C(void);
unsigned int StopI2C(void);
unsigned int IdleI2C(void);
unsigned int ACKStatus(void);
unsigned int NotAckI2C(void);
unsigned int AckI2C(void);
