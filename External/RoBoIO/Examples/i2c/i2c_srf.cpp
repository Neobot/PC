/*
 *  Example for Using RoBoard's I2C to access the SRF08 Ultrasonic Sensor
 */

#include <stdio.h>

#include <roboard.h>  // include this to use the RoBoIO library

void wait_ms(long t) {
    // This's a function for t-millisecond delay; implement this according to your platform!
    // The following is just a stupid (but simple) cross-platform implementation for the purpose of sample codes.

    volatile long i, j;
    for (i=0L; i<t; i++)
    for (j=0L; j<66666L; j++);
}

int main(void) {
    unsigned char buf[10];
    
    // first set the correct RoBoard version
    roboio_SetRBVer(RB_100);
    //roboio_SetRBVer(RB_100RD);  // if your RoBoard is RB-100RD
    //roboio_SetRBVer(RB_110);    // if your RoBoard is RB-110
    //roboio_SetRBVer(RB_050);    // if your RoBoard is RB-050

    if (i2c_Init(I2CMODE_AUTO, 400000L) == false)  // init I2C lib to 400Kbps
    {
        printf("ERROR: fail to init I2C lib (%s)!\n", roboio_GetErrMsg());
        return -1;
    }

    printf("send SRF ranging command...\n");
    buf[0] = 0x00;  //byte 1: set SRF08 command register
    buf[1] = 0x51;  //byte 2: set ranging Mode - result in cm
    i2c_Send(0xe0>>1, buf, 2);  // write 2 bytes to SRF08 whose I2C addr = 0xe0 >> 1
    // Note: the SRF08 I2C address 0xe0 actually contains the I2C r/w bit, so we shift it to get the correct I2C address

    wait_ms(100L);  // delay 100ms to wait SRF08's data ready

    printf("read SRF ranging data...\n");
    i2c_SensorRead(0xe0>>1, 0x02, buf, 2);  // first write 0x02 (set to read the 1st SRF08 range register) to SRF08
                                            // and then RESTART to read two bytes from SRF08

    printf("The ranging data = %u\n", (unsigned int)buf[0] * 256 + (unsigned int)buf[1]);
    printf("\nComplete.\n");

    i2c_Close();  // close I2C lib
    return 0;
}



/* =============================================================================

// Alternative code with error checking

int main(void) {
    unsigned char buf[10];
    
    // first set the correct RoBoard version
    roboio_SetRBVer(RB_100);
    //roboio_SetRBVer(RB_110);  // if your RoBoard is RB-110
    //roboio_SetRBVer(RB_050);  // if your RoBoard is RB-050

    if (i2c_Init(I2CMODE_AUTO, 400000L) == false)  // init I2C lib to 400Kbps
    {
        printf("ERROR: fail to init I2C lib (%s)!\n", roboio_GetErrMsg());
        return -1;
    }

    printf("send SRF ranging command...\n");
    buf[0] = 0x00;  //byte 1: set SRF command register
    buf[1] = 0x51;  //byte 2: set ranging Mode - result in cm
    if (i2c_Send(0xe0>>1, buf, 2) == false)
    // write 2 bytes to SRF08 whose I2C addr = 0xe0 >> 1
    // Note: the SRF I2C address 0xe0 actually contains the I2C r/w bit, so we shift it to get the correct I2C address
    {
        printf("ERROR: fail to write bytes to SRF08 (%s)!\n", roboio_GetErrMsg());
        goto FAIL_END;
    }
    
    wait_ms(100L);  // delay 100ms to wait SRF08's data ready

    printf("read SRF ranging data...\n");
    if (i2c_SensorRead(0xe0>>1, 0x02, buf, 2) == false)  // first write 0x02 (set to read the 1st SRF range register) to SRF08
    {                                                    // and then RESTART to read two bytes from SRF08
        printf("ERROR: fail to read SRF08 data (%s)!\n", roboio_GetErrMsg());
        goto FAIL_END;
    }
    printf("The ranging data = %u\n", ((unsigned int)buf[0]) * 256 + ((unsigned int)buf[1]));

    printf("\nComplete.\n");
    i2c_Close();  // close I2C lib
    return 0;

FAIL_END:

    i2c_Close();  // close I2C lib
    return -1;
}

============================================================================= */

