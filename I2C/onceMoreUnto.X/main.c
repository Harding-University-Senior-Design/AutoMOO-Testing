#include "mcc_generated_files/mcc.h"
#include "i2c.h"
#include <math.h>

#define RISING_EDGE_TRIGGER_SETTING 0b011
#define FALLING_EDGE_TRIGGER_SETTING 0b010

extern volatile bool mpuInterrupt;
extern volatile unsigned long timez;

void Init_MPU6050() {
    clearI2C();
    __delay_us(1000);

    I2C_Init();


    I2C_WriteReg(0x68, 0x6B, 0x00);
    I2C_WriteReg(0x68, 0x6B, 0x01);

    __delay_us(100);
}

void IC1_Initialize(void) {
    IC1CON1 = 0x0000;
    ANSBbits.ANSB13 = 0;
    TRISBbits.TRISB13 = 1;
    Nop();

    RPINR7bits.IC1R = 0b1101;

    while (IC1CON1bits.ICBNE) {
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

void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void) {
    if (IC1CON1bits.ICM == RISING_EDGE_TRIGGER_SETTING) {
        mpuInterrupt = true;
    }

    IFS0bits.IC1IF = 0;
}

void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void) {
    if (IFS0bits.T1IF == 1) {
        IFS0bits.T1IF = 0; //reset the interrupt flag
        timez++;
    }
}

void TimerInit(void) {
    PR1 = 0x07FF; //8191
    IPC0bits.T1IP = 1; //set interrupt priority
    T1CONbits.TCKPS = 0x03; //timer prescaler bits
    T1CONbits.TCS = 0; //using FOSC/2

    IFS0bits.T1IF = 0; //reset interrupt flag
    IEC0bits.T1IE = 1; //turn on the timer1 interrupt
    T1CONbits.TON = 1;
}

void read6SensorData(uint8_t addr, int *x, int *y, int *z) {
    uint8_t data[6];
    I2C_ReadNReg(0x68, addr, &data[0], 6);
    *x = data[0] << 8 | data[1];
    *y = data[2] << 8 | data[3];
    *z = data[4] << 8 | data[5];
}

int main(void) {
    SYSTEM_Initialize();
    Init_MPU6050();

    TimerInit();

    uint8_t b;

    I2C_ReadReg(MPU6050_I2C_ADDRESS, MPU6050_WHO_AM_I, &b);
    int count = -1000;

    MPU6050_address(0x68);

    while (1) {


        if (b == 0x68) {
            MPU6050_setup();

            while (true) {
                MPU6050_loop();
            }
        }
    }

    return -1;
}