#define __PIC24FJ128GA202__
#define FCY (_XTAL_FREQ / 2)

#include "mcc_generated_files/mcc.h"
#include "i2c.h"
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include <xc.h>
#include <math.h>

#define ACCELEROMETER_SENSITIVITY 6215.0
#define GYROSCOPE_SENSITIVITY 65.536
 
#define M_PI 3.14159265359	    
 
#define dt 0.01							// 10 ms sample rate!    
 
void ComplementaryFilter(short accData[3], short gyrData[3], float *pitch, float *roll)
{
    float pitchAcc, rollAcc;               
 
    // Integrate the gyroscope data -> int(angularSpeed) = angle
    *pitch += ((float)gyrData[0] / GYROSCOPE_SENSITIVITY) * dt; // Angle around the X-axis
    *roll -= ((float)gyrData[1] / GYROSCOPE_SENSITIVITY) * dt;    // Angle around the Y-axis
 
    // Compensate for drift with accelerometer data if !bullshit
    // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
    int forceMagnitudeApprox = abs(accData[0]) + abs(accData[1]) + abs(accData[2]);
    if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
    {
	// Turning around the X axis results in a vector on the Y-axis
        pitchAcc = atan2f((float)accData[1], (float)accData[2]) * 180 / M_PI;
        *pitch = *pitch * 0.98 + pitchAcc * 0.02;
 
	// Turning around the Y axis results in a vector on the X-axis
        rollAcc = atan2f((float)accData[0], (float)accData[2]) * 180 / M_PI;
        *roll = *roll * 0.98 + rollAcc * 0.02;
    }
} 


void readGyro(int *accelX, int *accelY, int *accelZ)
{
    uint8_t accel[6];
       
    // I2C_ReadReg(0x68, 0x43, &accel[0]);
    // I2C_ReadReg(0x68, 0x44, &accel[1]);
    // I2C_ReadReg(0x68, 0x45, &accel[2]);
    // I2C_ReadReg(0x68, 0x46, &accel[3]);
    // I2C_ReadReg(0x68, 0x47, &accel[4]);
    // I2C_ReadReg(0x68, 0x48, &accel[5]);

    I2C_ReadNReg(0x68, 0x43, &accel[0], 6);
    *accelX = accel[0] << 8 | accel[1];
    *accelY = accel[2] << 8 | accel[3];
    *accelZ = accel[4] << 8 | accel[5];
}


int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    __delay_ms(100);
    I2C_Init();
    __delay_ms(100);

    I2C_WriteReg(0x68,0x6B,0x00);
    __delay_ms(100);
    
    I2C_WriteReg(0x68,0x1C,0x00);
    __delay_ms(100);
    
    while (1)
    {
        int x,y,z;

        // readGyro(&x,&y,&z);

        x = gg[0] << 8 | gg[1];
        y = gg[2] << 8 | gg[3];
        z = gg[4] << 8 | gg[5];
        
        printf("please work now %i | %i | %i \r\n",x, y, z);
        __delay_ms(100);
    }

    return -1;
}
