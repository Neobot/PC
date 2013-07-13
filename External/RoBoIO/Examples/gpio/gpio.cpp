/*
 *  Example for Accessing RoBoard's GPIO
 */

#include <stdio.h>

#include <roboard.h>  // include this to use the RoBoIO library

int main(void) {
    int val;
    
    // first set the correct RoBoard version
    roboio_SetRBVer(RB_100);
    //roboio_SetRBVer(RB_100RD);  // if your RoBoard is RB-100RD
    //roboio_SetRBVer(RB_110);    // if your RoBoard is RB-110
    //roboio_SetRBVer(RB_050);    // if your RoBoard is RB-050

    if (rcservo_Init(RCSERVO_USENOPIN) == false)  // set all PWM/GPIO pins as GPIO mode
    {
        printf("ERROR: fail to init RC Servo lib (%s)!\n", roboio_GetErrMsg());
        return -1;
    }

    printf("Press ENTER to read digital input on pin S1...\n"); getchar();
    val = rcservo_InPin(RCSERVO_PINS1);
    if (val == 0)
        printf("    The result = LOW\n\n");
    else
        printf("    The result = HIGH\n\n");

    printf("Press ENTER to output HIGH on pin S2...\n"); getchar();
    rcservo_OutPin(RCSERVO_PINS2, 1);
    printf("    now you should get HIGH when measuring pin S2\n\n");

    printf("Press ENTER to output LOW on pin S2...\n"); getchar();
    rcservo_OutPin(RCSERVO_PINS2, 0);
    printf("    now you should get LOW when measuring pin S2\n\n");

    rcservo_Close();  // close RC Servo lib
    return 0;
}

