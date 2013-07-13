/*
 *  Example for Accessing RoBoard's PWM
 */

#include <stdio.h>

#include <roboard.h>  // include this to use the RoBoIO library

int main(void) {

	// first set the correct RoBoard version
    roboio_SetRBVer(RB_100);
    //roboio_SetRBVer(RB_100RD);  // if your RoBoard is RB-100RD
    //roboio_SetRBVer(RB_110);    // if your RoBoard is RB-110
    //roboio_SetRBVer(RB_050);    // if your RoBoard is RB-050

    if (rcservo_Init(RCSERVO_USEPINS1 + RCSERVO_USEPINS2) == false)  // set PWM/GPIO pins S1 & S2 as Servo mode
    {
        printf("ERROR: fail to init RC Servo lib (%s)!\n", roboio_GetErrMsg());
        return -1;
    }

    rcservo_EnterPWMMode();  // make all servo-mode pins go into PWM mode

    printf("Send PWM pulses of period 10ms duty 1500us on pin S1...\n");
    rcservo_SendCPWM(RCSERVO_PINS1, 10000L, 1500L);

    printf("Send PWM pulses of period 5ms duty 800us on pin S2...\n");
    rcservo_SendCPWM(RCSERVO_PINS2, 5000L, 800L);

    printf("Press ENTER to change PWM output.\n"); getchar();

    printf("Send PWM pulses of period 20ms duty 2300us on pin S1...\n");
    rcservo_SendCPWM(RCSERVO_PINS1, 20000L, 2300L);

    printf("Send PWM pulses of period 10ms duty 1000us on pin S2...\n");
    rcservo_SendCPWM(RCSERVO_PINS2, 10000L, 1000L);

    printf("Press ENTER to stop.\n"); getchar();

    rcservo_StopPWM(RCSERVO_PINS1);
    rcservo_StopPWM(RCSERVO_PINS2);

    rcservo_Close();  // close RC Servo lib
    return 0;
}

