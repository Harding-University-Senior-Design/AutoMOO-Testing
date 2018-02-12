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
        printf("Hello, World!\r\n");
        __delay_ms(100);
    }

    return -1;
}
