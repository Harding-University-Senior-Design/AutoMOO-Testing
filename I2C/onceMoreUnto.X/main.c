#include "mcc_generated_files/mcc.h"
#include "i2c.h"
#include <math.h>

#define RISING_EDGE_TRIGGER_SETTING 0b011
#define FALLING_EDGE_TRIGGER_SETTING 0b010
#define NUM_CAL_READINGS 10
#define GYRO_SCALE 131.0
#define ACCEL_SCALE 16384.0

void Init_MPU6050()
{
    clearI2C();
    I2C_Init();
    I2C_WriteReg(0x68, 0x6B, 0x00);
    I2C_WriteReg(0x68, 0x6B, 0x01);

    __delay_us(100);
}

void IC1_Initialize(void)
{
    IC1CON1 = 0x0000;
    ANSBbits.ANSB13 = 0;
    TRISBbits.TRISB13 = 1;
    Nop();

    RPINR7bits.IC1R = 0b1101;

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

void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void)
{
    if (IC1CON1bits.ICM == RISING_EDGE_TRIGGER_SETTING)
    {
        mpuInterrupt = true;
    }

    IFS0bits.IC1IF = 0;
}

void PWM_Module_Initialize(PWM_Module *left_motor, PWM_Module *right_motor)
{
    ANSBbits.ANSB0 = 0;
    ANSBbits.ANSB1 = 0;
    ANSBbits.ANSB14 = 0;
    ANSBbits.ANSB15 = 0;

    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB3 = 0;

    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB14 = 0;
    TRISBbits.TRISB15 = 0;

    left_motor->Initialize = PWM_OC1_Initialize;
    left_motor->GetDutyCycle = PWM_Get_OC1_DutyCycle;
    left_motor->GetFrequency = PWM_Get_OC1_Frequency;
    left_motor->UpdateDutyCycle = PWM_Update_OC1_DutyCycle;
    left_motor->UpdateFrequency = PWM_Update_OC1_Frequency;

    right_motor->Initialize = PWM_OC2_Initialize;
    right_motor->GetDutyCycle = PWM_Get_OC2_DutyCycle;
    right_motor->GetFrequency = PWM_Get_OC2_Frequency;
    right_motor->UpdateDutyCycle = PWM_Update_OC2_DutyCycle;
    right_motor->UpdateFrequency = PWM_Update_OC2_Frequency;
}

int main(void)
{
    SYSTEM_Initialize();
    Init_MPU6050();


    PWM_Module Left_Motor;
    PWM_Module Right_Motor;
    PWM_Module_Initialize(&Left_Motor, &Right_Motor);

    Left_Motor.Initialize(&Left_Motor);
    Right_Motor.Initialize(&Right_Motor);

    Left_Motor.frequency = 20000;
    Left_Motor.UpdateFrequency(&Left_Motor);

    Right_Motor.frequency = 20000;
    Right_Motor.UpdateFrequency(&Right_Motor);

    Left_Motor.dutyCyclePercentage = 25;
    Left_Motor.UpdateDutyCycle(&Left_Motor);

    Right_Motor.dutyCyclePercentage = 25;
    Right_Motor.UpdateDutyCycle(&Right_Motor);

    LATBbits.LATB14 = 1;
    LATBbits.LATB15 = 0;

    __delay_ms(4000);

    uint8_t b;
    I2C_ReadReg(MPU6050_I2C_ADDRESS, MPU6050_WHO_AM_I, &b);

    int rightMotorSpeed = 25;
    int leftMotorSpeed = 25;
    Left_Motor.dutyCyclePercentage = leftMotorSpeed;
    Left_Motor.UpdateDutyCycle(&Left_Motor);
    Right_Motor.dutyCyclePercentage = rightMotorSpeed;
    Right_Motor.UpdateDutyCycle(&Right_Motor);

    if (b == 0x68)
    {
        MPU6050_setup();

        while (true)
        {
            // MPU6050_loop();
        }
    }
    return -1;
}