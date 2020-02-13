#ifndef _LINEDETECTOR_H_
#define _LINEDETECTOR_H_

#include "AnalogSensor.h"
#include "Event.h"
#include "Timer.h"

#define LEFT_THRESHOLD_STR "Set Left Line Threshold"
#define RIGHT_THRESHOLD_STR "Set Right Line Threshold"

typedef enum
{
    LL_NotDetected,
    LL_Left,
    LL_Right
} LineLocation_t;

typedef struct
{
    AnalogSensor_t leftLineSensor;
    AnalogSensor_t rightLineSensor;
    Event_t *lineDetectedEvent;
    EventSubscription_t newMessageSubscription;
    LineLocation_t lineLocation;
    Timer_t timer;
} LineDetector_t;

void LineDetector_Init(LineDetector_t *instance, Event_t *lineDetectedEvent, Event_t *newSerialMessageEvent, TimerModule_t *timerModule);

#endif
