#define FCY (_XTAL_FREQ / 2)

#include <xc.h>
#include <stdio.h>
#include <libpic30.h>
#include "mcc_generated_files/mcc.h"
 
int main(void)
{
    SYSTEM_Initialize();

    while (1)
    {
        printf("HI");
        __delay_ms(1000);
    }

    return -1;
}
