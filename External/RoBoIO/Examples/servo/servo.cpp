/*
 *  Example for Moving RC servos
 */

#include <stdio.h>

#include <roboard.h>  // include this to use the RoBoIO library

int main(void) {
    unsigned long home[32]  = {0L};
    unsigned long frame[32] = {0L};
    
    // first set the correct RoBoard version
    roboio_SetRBVer(RB_100);
    //roboio_SetRBVer(RB_100RD);  // if your RoBoard is RB-100RD
    //roboio_SetRBVer(RB_110);    // if your RoBoard is RB-110
    //roboio_SetRBVer(RB_050);    // if your RoBoard is RB-050

    rcservo_SetServo(RCSERVO_PINS1, RCSERVO_SERVO_DEFAULT_NOFB);     // select the servo model on pin S1 as non-feedback servo
    rcservo_SetServo(RCSERVO_PINS2, RCSERVO_SERVO_DEFAULT_NOFB);     // select the servo model on pin S2 as non-feedback servo
    if (rcservo_Init(RCSERVO_USEPINS1 + RCSERVO_USEPINS2) == false)  // set PWM/GPIO pins S1 & S2 as Servo mode
    {
        printf("ERROR: fail to init RC Servo lib (%s)!\n", roboio_GetErrMsg());
        return -1;
    }

    home[0] = home[1] = 1500L;  // set the initial home position of all servos as 1500us
    rcservo_EnterPlayMode_HOME(home);  // enter Action Playing Mode for moving servos

    printf("press ENTER to move servo on pins S1 & S2.\n"); getchar();
    frame[0] = 900L;   // move the servo on pin S1 to position 900us
    frame[1] = 1800L;  // move the servo on pin S2 to position 1800us
    rcservo_MoveTo(frame, 2000L);  // move servos to the target positions in 2000ms

    printf("press ENTER to move only the servo on pin S1.\n"); getchar();
    rcservo_MoveOne(RCSERVO_PINS1, 1800L, 500L);  // move the servo to position 1800us in 300ms

    printf("press ENTER to move servo on pins S1 & S2.\n"); getchar();
    frame[0] = 900L;   // move the servo on pin S1 to position 900us
    frame[1] = 900L;   // move the servo on pin S2 to position 900us
    rcservo_MoveTo(frame, 3000L);  // move servos to the target positions in 3000ms

    printf("Complete.\n");

    rcservo_Close();  // close RC Servo lib
    return 0;
}

