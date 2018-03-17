#define __PIC24FJ128GA202__
#define FCY (_XTAL_FREQ / 2)
#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include <libpic30.h>
#include <xc.h>

#define FCY (_XTAL_FREQ / 2)
#define I2C_BAUDRATE    (unsigned long)100000 //default baud rate 100kHz
#define I2C_ERROR           -1
#define I2C_OK              1

void I2C_Init(void);
int setBaudRate(void);
int I2C_WriteReg(char dev_addr, char reg_addr, char value);
int I2C_ReadReg(char dev_addr, char reg_addr, uint8_t *value);
int I2C_ReadNReg(char dev_addr, uint8_t reg_addr, uint8_t *value, int numRegisters);