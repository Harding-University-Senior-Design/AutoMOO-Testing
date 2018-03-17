#include "i2c.h"

void I2C_Init(void)
{
    I2C1CONLbits.I2CEN = 0;	
	I2C1CONLbits.I2CSIDL 	= 0;
	I2C1CONLbits.A10M		= 0;
	I2C1CONLbits.DISSLW 	= 1;
	I2C1CONLbits.SMEN 	= 0;
    I2C1BRG              = setBaudRate();    
    __delay_ms(1);
    I2C1CONLbits.I2CEN 	= 1;
}

int setBaudRate(){
    return (FCY/I2C_BAUDRATE - FCY/1111111) - 1;
}

int I2C_WriteReg(char dev_addr, char reg_addr, char value)
{
    char wr_dev_addr = dev_addr << 1;
    // Send I2C start condition
	I2C1CONLbits.SEN = 1;			
	while(I2C1CONLbits.SEN == 1);
	// Send I2C device address on the bus for write operation
	I2C1TRN = wr_dev_addr;			
	while(I2C1STATbits.TRSTAT);			
	if (I2C1STATbits.ACKSTAT)				
	{								
		I2C1CONLbits.PEN = 1;
		while(I2C1CONLbits.PEN);			
		return I2C_ERROR;					
	}
    // Send register address on the bus
	I2C1TRN = reg_addr;
	while(I2C1STATbits.TRSTAT);
	if (I2C1STATbits.ACKSTAT)
	{
		I2C1CONLbits.PEN = 1;
		while(I2C1CONLbits.PEN);
		return I2C_ERROR;
	}
	// Send register value on the bus    
	I2C1TRN = value;
	while(I2C1STATbits.TRSTAT);
	if (I2C1STATbits.ACKSTAT)
	{
		I2C1CONLbits.PEN = 1;
		while(I2C1CONLbits.PEN);
		return I2C_ERROR;
	}
	/// Send I2C stop condition
	I2C1CONLbits.PEN = 1;
	while(I2C1CONLbits.PEN);
	return I2C_OK;
}



int I2C_ReadReg(char dev_addr, char reg_addr, uint8_t *value)
{
    char wr_dev_addr = dev_addr << 1;
    char rd_dev_addr = (dev_addr << 1) | 0x01;
    // Send I2C start condition
	I2C1CONLbits.SEN = 1;	
	while(I2C1CONLbits.SEN == 1);
	// Send I2C device address on the bus for write operation
	I2C1TRN = wr_dev_addr;
	while(I2C1STATbits.TRSTAT);
	if (I2C1STATbits.ACKSTAT)
	{
		I2C1CONLbits.PEN = 1;
		while(I2C1CONLbits.PEN);
		return I2C_ERROR;
	}
    // Send I2C register address on the bus 
	I2C1TRN = reg_addr;
	while(I2C1STATbits.TRSTAT);
	if (I2C1STATbits.ACKSTAT)
	{	
		I2C1CONLbits.PEN = 1;
		while(I2C1CONLbits.PEN);
		return I2C_ERROR;
	}
    // Send I2C restart condition
    I2C1CONLbits.RSEN = 1;
    while(I2C1CONLbits.RSEN == 1);	
    // Send I2C device address on the bus for read operation
    I2C1TRN = rd_dev_addr;
	while(I2C1STATbits.TRSTAT);
	if (I2C1STATbits.ACKSTAT)
	{
		I2C1CONLbits.PEN = 1;
		while(I2C1CONLbits.PEN);
		return I2C_ERROR;	
	}
    // Enable I2C clock for read operation
	I2C1CONLbits.RCEN = 1;
    while(!I2C1STATbits.RBF);
    // Retrieve value from I2C register
	*value = I2C1RCV;	
	// Send I2C stop condition
	I2C1CONLbits.PEN = 1;
	while(I2C1CONLbits.PEN);
	return I2C_OK;
}

int I2C_ReadNReg(char dev_addr, uint8_t reg_addr, uint8_t *value, int numRegisters)
{
	uint8_t accel[numRegisters];

	int i = 0, returnValue;
	uint8_t currentRegAddr = reg_addr;

	for (i; i < numRegisters; i++, currentRegAddr++)
	{
		returnValue = I2C_ReadReg(dev_addr, currentRegAddr, &accel[i]);

		*(value + i) = accel[i];
	}


	return returnValue;
}