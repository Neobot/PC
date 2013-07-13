/*
 *  Example for Accessing RoBoard's ADC
 */

#include <stdio.h>

#include <roboard.h>  // include this to use the RoBoIO library

int main(void) {
    int i, val;
    
    // first set the correct RoBoard version
    roboio_SetRBVer(RB_100);
    //roboio_SetRBVer(RB_100RD);  // if your RoBoard is RB-100RD
    //roboio_SetRBVer(RB_110);    // if your RoBoard is RB-110
    //roboio_SetRBVer(RB_050);    // if your RoBoard is RB-050

    if (spi_Init(SPICLK_21400KHZ) == false)  // init SPI lib for accessing the A/D converter
    {
        printf("ERROR: fail to init SPI lib (%s)!\n", roboio_GetErrMsg());
        return -1;
    }

    for (i=0; i<8; i++)
    {
        val = adc_ReadCH(i);  // read the ADC input value on channel i

        if (val == ADC_READFAIL)
            printf("ERROR: fail to read ADC value on channel %d!\n", i);
        else
            printf("ADC value on channel %d = %d (equiv. to %1.2fV)\n", i, val, 5.0*(float)val/1023.0);
    }

    spi_Close();  // close SPI lib
    return 0;
}

