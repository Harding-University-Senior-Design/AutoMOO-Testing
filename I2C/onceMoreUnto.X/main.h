#define FCY (_XTAL_FREQ / 2)
#define FP  (_XTAL_FREQ / 2)

#include <xc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpic30.h>
#include "mcc_generated_files/mcc.h"
#include "MPU6050.h"
#include "PWM.h"

#define M_PI 3.14159


#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

#define DMP_Hz      20
#define HZ_val      200/DMP_Hz-1


void IC1_Initialize(void);