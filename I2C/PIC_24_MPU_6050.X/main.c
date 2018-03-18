// #define __PIC24FJ128GA202__
#define FCY (_XTAL_FREQ / 2)

#include "mcc_generated_files/mcc.h"
#include "i2c.h"
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include <xc.h>
#include <math.h>

#define MPU_6050_ADDR 0x68
#define MPU_6050_GYRO_ST_ADDR 0x43
#define MPU_6050_ACEL_ST_ADDR 0x3B
#define ACCELEROMETER_SENSITIVITY 6215.0
#define GYROSCOPE_SENSITIVITY 65.536
 
#define M_PI 3.14159265359	    
 
#define dt 0.01							// 10 ms sample rate!    

typedef struct MPU_6050_Module
{
	int x,
		y,
		z,
		xOffset,
		yOffset,
		zOffset;
}MPU_6050_Module;

typedef struct MPU_6050
{
	MPU_6050_Module	gyro,
					accel;

}MPU_6050;


void readMotionData(int *dataX, int *dataY, int *dataZ, uint8_t moduleType )
{
    uint8_t data[6];

    I2C_ReadNReg(MPU_6050_ADDR, moduleType, &data[0], 6);
    *dataX = data[0] << 8 | data[1];
    *dataY = data[2] << 8 | data[3];
    *dataZ = data[4] << 8 | data[5];
}

void calibrateModule(int samples, int sampleDelay, MPU_6050_Module *imu, uint8_t moduleType)
{
	int x_offset_temp = 0,
		y_offset_temp = 0,
		z_offset_temp = 0,
		x, y, z;

	readMotionData(&x, &y, &z, moduleType);

	int i = 0;
	for (i = 0; i < samples; i++)
	{
		// __delay_ms(sampleDelay);
		readMotionData(&x, &y, &z, moduleType);
		x_offset_temp += x;
		y_offset_temp += y;
		z_offset_temp += z;
	}

	imu->xOffset= abs(x_offset_temp) / samples;
	imu->yOffset = abs(y_offset_temp) / samples;
	imu->zOffset = abs(z_offset_temp) / samples;
	if (x_offset_temp > 0)imu->xOffset = -imu->xOffset;
	if (y_offset_temp > 0)imu->yOffset = -imu->yOffset;
	if (z_offset_temp > 0)imu->zOffset = -imu->zOffset;
}

void MPU_6050_Initialize(MPU_6050 *imu)
{
    I2C_Init();
    __delay_ms(100);
    I2C_WriteReg(0x68,0x6B,0x00);
    __delay_ms(100);
    I2C_WriteReg(0x68,0x1C,0x00);
    __delay_ms(100);
    // calibrateModule(200, 10, &imu->accel, MPU_6050_ACEL_ST_ADDR);
	// calibrateModule(200, 10, &imu->gyro,  MPU_6050_GYRO_ST_ADDR);
}


int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    __delay_ms(1000);
    
	MPU_6050 imu;
    printf("initializing\r\n");
	MPU_6050_Initialize(&imu);    

	long x_offset_temp = 0,
	y_offset_temp = 0,
	z_offset_temp = 0;
	int x,y,z;

	readMotionData(&x, &y, &z, MPU_6050_GYRO_ST_ADDR);

	int i;
	for (i = 0; i < 50; i++)
	{
		// __delay_ms(sampleDelay);
		readMotionData(&x, &y, &z, MPU_6050_GYRO_ST_ADDR);
		x_offset_temp += x;
		y_offset_temp += y;
		z_offset_temp += z;
	}

	int g,h,j;
	g = x_offset_temp / 50;
	h = y_offset_temp / 50;
	j = z_offset_temp / 50;


		//***********************
    x_offset_temp = 0,
	y_offset_temp = 0,
	z_offset_temp = 0;


	readMotionData(&x, &y, &z, MPU_6050_ACEL_ST_ADDR);

	for (i = 0; i < 50; i++)
	{
		// __delay_ms(sampleDelay);
		readMotionData(&x, &y, &z, MPU_6050_ACEL_ST_ADDR);
		x_offset_temp += x;
		y_offset_temp += y;
		z_offset_temp += z;
	}

	int b,n,m;
	b = x_offset_temp / 50;
	n = y_offset_temp / 50;
	m = z_offset_temp / 50;


    
    while (1)
    {
        int x,y,z,r,t,s;
        readMotionData(&x,&y,&z,MPU_6050_GYRO_ST_ADDR);
        readMotionData(&r,&t,&s,MPU_6050_ACEL_ST_ADDR);


        printf("%i | %i | %i | %i | %i | %i\r\n",x-g,y-h,z-j, r-b, t-n, s-m);
		__delay_ms(100);
    }

    return -1;
}
