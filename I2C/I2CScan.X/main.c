#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include "drvI2C.h"

int main(void)
{
    SYSTEM_Initialize();
    drvI2CInit();
    
    UINT8 x = 0x00;
    for (x; x < 0xAA; x+=2)
    {
        if(drvI2CWriteByte(0x00, 0x00, x))
        {
            printf("I2C Device Found at: %x\r\n",x);
        }
        else
        {
            printf("Nothing found at %x\r\n",x);
        }
    }
    
    while (1)
    {
//        Main Loop
    }

    return -1;
}