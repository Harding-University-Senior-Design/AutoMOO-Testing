#include "main.h"
#include "i2c.h"
#include "MPU6050.h"
uint8_t cccee[14];

#define RISING_EDGE_TRIGGER_SETTING 0b011
#define FALLING_EDGE_TRIGGER_SETTING 0b010

void IC1_Initialize(void)
{
    IC1CON1 = 0x0000;
    ANSBbits.ANSB14 = 0;
	TRISBbits.TRISB14 = 1;
	Nop();
    
    RPINR7bits.IC1R = 0b1110;

    while (IC1CON1bits.ICBNE)
    {
        int junk = IC1BUF;
            junk = 2;
    }
    
    T1CON = 0b0000000000000000;
    T1CONbits.TCKPS = 0b10;
    T1CONbits.TCS = 0b0;
    T1CONbits.TON = 1;
    IC1CON2bits.SYNCSEL = 0b00000;
    IC1CON1bits.ICTSEL = 0b100;
    IC1CON1bits.ICI = 0b00;
    IC1CON1bits.ICM = RISING_EDGE_TRIGGER_SETTING;
    IPC0bits.IC1IP = 1;
    IFS0bits.IC1IF = false;
    IEC0bits.IC1IE = true;
}

extern volatile bool mpuInterrupt;

void __attribute__ ((__interrupt__, auto_psv)) _IC1Interrupt(void)
{
    if (IC1CON1bits.ICM == RISING_EDGE_TRIGGER_SETTING)
    {
        mpuInterrupt   = true;
    }

    IFS0bits.IC1IF = 0;
}

int main(void)
{
    SYSTEM_Initialize();
    __delay_ms(1000);
     I2C_Init();
    
    printf("\n\rGWelcome_ MPU6050 DMP test \n");

    printf("\t MPU6050_WHO_AM_I:[%x]\n\r",I2C2Read(MPU6050_I2C_ADDRESS,MPU6050_WHO_AM_I)); 

    if(I2C2Read(MPU6050_I2C_ADDRESS,MPU6050_WHO_AM_I)==0x68)
    {
        I2C_WriteReg(0x68,0x6B,0x00);
        MPU6050_setXGyroOffset(220);
        MPU6050_setYGyroOffset(76);
        MPU6050_setZGyroOffset(-85);
        MPU6050_setZAccelOffset(1788); // 1688 factory default for my test chip

        MPU6050_setup();
       

        // while(true) 
        // {

        //     MPU6050_loop();
        //     ClrWdt();
        // }
    }

    printf("MPU6050 Not found!!! \n");

    while(1) Idle();

    return -1;
}
