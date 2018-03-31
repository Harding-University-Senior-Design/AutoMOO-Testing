#include "i2c.h"

void I2C_Init(void) 
{
    I2C1CONLbits.I2CEN = 0;
    I2C1CONLbits.I2CSIDL = 0;
    I2C1CONLbits.A10M = 0;
    I2C1CONLbits.DISSLW = 1;
    I2C1CONLbits.SMEN = 0;
    I2C1BRG = setBaudRate();
    __delay_ms(1);
    I2C1CONLbits.I2CEN = 1;
}

unsigned int StartI2C(void) 
{
    //This function generates an I2C start condition and returns status 
    //of the Start.

    I2C1CONLbits.SEN = 1; //Generate Start COndition
    while (I2C1CONLbits.SEN); //Wait for Start COndition
    //return(I2C2STATbits.S);	//Optionally return status
    return 0;
}

unsigned int RestartI2C(void) 
{
    //This function generates an I2C Restart condition and returns status 
    //of the Restart.

    I2C1CONLbits.RSEN = 1; //Generate Restart		
    while (I2C1CONLbits.RSEN); //Wait for restart	
    //return(I2C2STATbits.S);	//Optional - return status
    return 0;
}

unsigned int StopI2C(void) 
{
    //This function generates an I2C stop condition and returns status 
    //of the Stop.

    I2C1CONLbits.PEN = 1; //Generate Stop Condition
    while (I2C1CONLbits.PEN); //Wait for Stop
    //return(I2C2STATbits.P);	//Optional - return status
    return 0;
}

unsigned int IdleI2C(void) 
{
    while (I2C1STATbits.TRSTAT); //Wait for bus Idle
    return 0;
}

unsigned int ACKStatus(void) 
{
    return (!I2C1STATbits.ACKSTAT); //Return Ack Status
}

unsigned int NotAckI2C(void) 
{
    I2C1CONLbits.ACKDT = 1; //Set for NotACk
    I2C1CONLbits.ACKEN = 1;
    while (I2C1CONLbits.ACKEN); //wait for ACK to complete
    I2C1CONLbits.ACKDT = 0; //Set for NotACk
    return 0;
}

unsigned int AckI2C(void) 
{
    I2C1CONLbits.ACKDT = 0; //Set for ACk
    I2C1CONLbits.ACKEN = 1;
    while (I2C1CONLbits.ACKEN); //wait for ACK to complete
    return 0;
}



int setBaudRate() {
    return (FCY / I2C_BAUDRATE - FCY / 1111111) - 1;
}

int I2C_WriteReg(char dev_addr, char reg_addr, char value) {
    char wr_dev_addr = dev_addr << 1;
    // Send I2C start condition
    I2C1CONLbits.SEN = 1;
    while (I2C1CONLbits.SEN == 1);
    // Send I2C device address on the bus for write operation
    I2C1TRN = wr_dev_addr;
    while (I2C1STATbits.TRSTAT);
    if (I2C1STATbits.ACKSTAT) {
        I2C1CONLbits.PEN = 1;
        while (I2C1CONLbits.PEN);
        return I2C_ERROR;
    }
    // Send register address on the bus
    I2C1TRN = reg_addr;
    while (I2C1STATbits.TRSTAT);
    if (I2C1STATbits.ACKSTAT) {
        I2C1CONLbits.PEN = 1;
        while (I2C1CONLbits.PEN);
        return I2C_ERROR;
    }
    // Send register value on the bus    
    I2C1TRN = value;
    while (I2C1STATbits.TRSTAT);
    if (I2C1STATbits.ACKSTAT) {
        I2C1CONLbits.PEN = 1;
        while (I2C1CONLbits.PEN);
        return I2C_ERROR;
    }
    /// Send I2C stop condition
    I2C1CONLbits.PEN = 1;
    while (I2C1CONLbits.PEN);
    return I2C_OK;
}

int I2C_ReadReg(char dev_addr, char reg_addr, uint8_t *value) {
    char wr_dev_addr = dev_addr << 1;
    char rd_dev_addr = (dev_addr << 1) | 0x01;
    // Send I2C start condition
    I2C1CONLbits.SEN = 1;
    while (I2C1CONLbits.SEN == 1);
    // Send I2C device address on the bus for write operation
    I2C1TRN = wr_dev_addr;
    while (I2C1STATbits.TRSTAT);
    if (I2C1STATbits.ACKSTAT) {
        I2C1CONLbits.PEN = 1;
        while (I2C1CONLbits.PEN);
        return I2C_ERROR;
    }
    // Send I2C register address on the bus 
    I2C1TRN = reg_addr;
    while (I2C1STATbits.TRSTAT);
    if (I2C1STATbits.ACKSTAT) {
        I2C1CONLbits.PEN = 1;
        while (I2C1CONLbits.PEN);
        return I2C_ERROR;
    }
    // Send I2C restart condition
    I2C1CONLbits.RSEN = 1;
    while (I2C1CONLbits.RSEN == 1);
    // Send I2C device address on the bus for read operation
    I2C1TRN = rd_dev_addr;
    while (I2C1STATbits.TRSTAT);
    if (I2C1STATbits.ACKSTAT) {
        I2C1CONLbits.PEN = 1;
        while (I2C1CONLbits.PEN);
        return I2C_ERROR;
    }
    // Enable I2C clock for read operation
    I2C1CONLbits.RCEN = 1;
    while (!I2C1STATbits.RBF);
    // Retrieve value from I2C register
    *value = I2C1RCV;
    // Send I2C stop condition
    I2C1CONLbits.PEN = 1;
    while (I2C1CONLbits.PEN);
    return I2C_OK;
}



void I2C2Write(uint8_t devAddr, uint16_t regAddr, uint8_t data) {
    I2C_WriteReg(devAddr, regAddr, data);
}

void I2C2writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) 
{
    //    I2C2writeBit(devAddr, MPU6050_RA_XG_OFFS_TC, MPU6050_TC_OTP_BNK_VLD_BIT, enabled);

    uint8_t b;
    I2C_ReadReg(devAddr, regAddr, &b);
    printf("%i",b);

    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    printf(" - %i",b);
    I2C_WriteReg(devAddr, regAddr, b);

    I2C_ReadReg(devAddr, regAddr, &b);
    printf(" - %i\r\n",b);

}

bool I2C2writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b = I2C2Read(devAddr, regAddr);
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1); // shift data into correct position
    data &= mask; // zero all non-important bits in data
    b &= ~(mask); // zero all important bits in existing byte
    b |= data; // combine data with existing byte
    I2C2Write(devAddr, regAddr, b);
    return true;
}

/**************************************************************************************/
void I2C2writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t len, uint8_t *dptr) 
{
    while (len--) 
    {
        I2C2Write(devAddr, regAddr++, *dptr++);
    }
}

/**************************************************************************************/
void I2C2writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data) 
{
    I2C2Write(devAddr, regAddr++, (data >> 8)&0xFF);
    I2C2Write(devAddr, regAddr, data & 0xFF);
}

uint8_t I2C2Read(uint8_t devAddr, uint16_t regAddr) 
{
    uint8_t data;
    I2C_ReadReg(devAddr, regAddr, &data);
    return data;
}

uint8_t I2C2readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data) 
{
    uint8_t b = 0;
    uint8_t count = I2C2Read(devAddr, regAddr);
    *data = b & (1 << bitNum);
    return count;
}

/**************************************************************************************/
uint8_t I2C2readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) 
{
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t count = 0, b = 0;
    b = I2C2Read(devAddr, regAddr);
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    b &= mask;
    b >>= (bitStart - length + 1);
    *data = b;
    return count;
}

/**************************************************************************************/
void I2C2readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t len, uint8_t *dptr) 
{

    int i;
    uint8_t currentRegAddr = regAddr;

    for (i = 0; i < len; i++, currentRegAddr++) 
    {
        uint8_t reg;

        I2C_ReadReg(devAddr, currentRegAddr, &reg);

        *(dptr + i) = reg;
    }
}
