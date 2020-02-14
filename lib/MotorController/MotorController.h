#ifndef _MOTORCONTROLLER_H_
#define _MOTORCONTROLLER_H_

#include "Event.h"
#include "I_Hardware.h"

#define MOVE_CHAR 'M'
#define X_CHAR 'X'
#define Y_CHAR 'Y'

typedef enum
{
    MS_Stop,
    MS_GoSlow,
    MS_GoFast,
    MS_Reverse
} MotorSpeed_t;

// - PIVOT  : The threshold at which the pivot action starts
//            This threshold is measured in units on the Y-axis
//            away from the X-axis (y = 0). A greater value will assign
//            more of the joystick's range to pivot actions.
//            Allowable range: (0..127)
#define PIVOT 32.0;

typedef struct
{
    MotorSpeed_t left;
    MotorSpeed_t right;
} MotorRequest_t;

typedef struct
{
    EventSubscription_t newMessageSubscription;
    EventSubscription_t motorRequestSubscription;
    I_Hardware_t *hardware;
} MotorController_t;

void MotorController_Init(MotorController_t *instance, Event_t *motorRequestEvent, Event_t *newSerialMessageEvent, I_Hardware_t *hardware);
void MotorController_ParseMessage(MotorController_t *instance, char *message);

#endif
