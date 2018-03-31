#include "main.h"

#define I2C_BAUDRATE    (unsigned long)400000 //default baud rate 100kHz
#define I2C_ERROR        -1
#define I2C_OK           1

void I2C_Init(void);
int setBaudRate(void);
int I2C_WriteReg(char dev_addr, char reg_addr, char value);
int I2C_WriteNReg(char dev_addr, uint8_t reg_addr, uint8_t *value, int numRegisters);
int I2C_ReadReg(char dev_addr, char reg_addr, uint8_t *value);
int I2C_ReadNReg(char dev_addr, uint8_t reg_addr, uint8_t *value, int numRegisters);
int I2C_ReadSeqReg(char dev_addr, char reg_addr, uint8_t *value, uint8_t numRegisters);
int I2C_WriteSeqReg(char dev_addr, char reg_addr, uint8_t *value, uint8_t numRegisters);
void I2C_WriteWord(uint8_t devAddr,uint8_t regAddr,uint16_t data);
void I2C_WriteBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data); 
bool I2C_WriteBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
uint8_t I2C_ReadBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data);
uint8_t I2C_ReadBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
unsigned int RestartI2C(void);
