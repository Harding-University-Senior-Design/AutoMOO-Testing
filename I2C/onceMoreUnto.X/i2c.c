#include "i2c.h"

void I2C_Init(void) {
    I2C1CONLbits.I2CEN = 0;
    I2C1CONLbits.I2CSIDL = 0;
    I2C1CONLbits.A10M = 0;
    I2C1CONLbits.DISSLW = 1;
    I2C1CONLbits.SMEN = 0;
    I2C1BRG = setBaudRate();
    __delay_ms(1);
    I2C1CONLbits.I2CEN = 1;
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

int I2C_WriteNReg(char dev_addr, uint8_t reg_addr, uint8_t *value, int numRegisters) {
    int i = 0, returnValue;

    for (i = 0; i < numRegisters; i++, reg_addr++) {
        printf("%X ",value[i]);
        returnValue = I2C_WriteReg(dev_addr, reg_addr, value[i]);
        __delay_ms(1);
    }
    printf("\n");

    return returnValue;
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

int I2C_ReadNReg(char dev_addr, uint8_t reg_addr, uint8_t *value, int numRegisters) {
    int i = 0, returnValue;

    for (i = 0; i < numRegisters; i++, reg_addr++) {
        returnValue = I2C_ReadReg(dev_addr, reg_addr, (value + i));
        __delay_ms(1);

    }

    return returnValue;
}

void I2C_WriteWord(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
    I2C_WriteReg(devAddr, regAddr++, (data >> 8)&0xFF);
    I2C_WriteReg(devAddr, regAddr, data & 0xFF);
}

int I2C_WriteSeqReg(char dev_addr, char reg_addr, uint8_t *value, uint8_t numRegisters) {
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
    
    // printf("Writing: ");

    int x;
    for (x = 0; x < numRegisters; x++) {
        // Send register value on the bus  
        
        I2C1TRN = value[x];
        
        // printf("%X ", value[x]);

        while (I2C1STATbits.TBF);

        while (I2C1STATbits.TRSTAT);
        if (I2C1STATbits.ACKSTAT) 
        {
            I2C1CONLbits.PEN = 1;
            while (I2C1CONLbits.PEN);
            return I2C_ERROR;
        }
    }
    
    // printf("\n");


    /// Send I2C stop condition
    I2C1CONLbits.PEN = 1;
    while (I2C1CONLbits.PEN);
    return I2C_OK;
}

int I2C_ReadSeqReg(char dev_addr, char reg_addr, uint8_t *value, uint8_t numRegisters) {
    if (numRegisters > 0) {
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

        while (numRegisters--) {
            // Enable I2C clock for read operation
            I2C1CONLbits.RCEN = 1;
            while (!I2C1STATbits.RBF);
            // Retrieve value from I2C register
            *value++ = I2C1RCV;

            if (I2C1STATbits.BCL) {
                return -1;
            }

            if (numRegisters) {
                I2C1CONLbits.ACKDT = 0; //Set for ACk
                I2C1CONLbits.ACKEN = 1;
                while (I2C1CONLbits.ACKEN); //wait for ACK to complete
            }

            __delay_ms(10);
        }
        // Send I2C stop condition
        I2C1CONLbits.PEN = 1;
        while (I2C1CONLbits.PEN);
        return I2C_OK;
    }
    return -1;
}

void I2C_WriteBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;

    int result = I2C_ReadReg(devAddr, regAddr, &b);

    while (result != 1) {
        RestartI2C();
        __delay_ms(10);
        result = I2C_ReadReg(devAddr, regAddr, &b);
    }

    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));

    I2C_WriteReg(devAddr, regAddr, b);
}

unsigned int RestartI2C(void) {
    //This function generates an I2C Restart condition and returns status 
    //of the Restart.

    I2C1CONLbits.RSEN = 1; //Generate Restart		
    while (I2C1CONLbits.RSEN); //Wait for restart	
    //return(I2C2STATbits.S);	//Optional - return status
    return 0;
}

bool I2C_WriteBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b;
    I2C_ReadReg(devAddr, regAddr, &b);
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1); // shift data into correct position
    data &= mask; // zero all non-important bits in data
    b &= ~(mask); // zero all important bits in existing byte
    b |= data; // combine data with existing byte
    I2C_WriteReg(devAddr, regAddr, b);
    return true;
}

uint8_t I2C_ReadBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
    uint8_t b = 0;
    uint8_t count;
    I2C_ReadReg(devAddr, regAddr, &count);
    *data = b & (1 << bitNum);
    return count;
}

uint8_t I2C_ReadBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t count = 0, b = 0;
    I2C_ReadReg(devAddr, regAddr, &b);
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    b &= mask;
    b >>= (bitStart - length + 1);
    *data = b;
    return count;
}

