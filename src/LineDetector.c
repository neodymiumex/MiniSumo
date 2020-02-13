#include "LineDetector.h"
#include <string.h>

#define LINE_SENSOR_LEFT  8
#define LINE_SENSOR_RIGHT 9

static void ParseMessage(void *context, void *args)
{
    LineDetector_t *instance = (LineDetector_t *)context;
    char *message = (char *)args;
    char *leftThresholdsStr = LEFT_THRESHOLD_STR;
    char *rightThresholdStr = RIGHT_THRESHOLD_STR;

    if(strstr(message, leftThresholdsStr))
    {
        int leftThreshold;
        char *substr;

        leftThreshold = atoi(message + strlen(leftThresholdsStr) + 1);

        AnalogSensor_UpdateThreshold(&instance->leftLineSensor, leftThreshold);
    }
    else if(strstr(message, rightThresholdStr))
    {
        int rightThreshold;

        rightThreshold = atoi(message + strlen(rightThresholdStr) + 1);

        AnalogSensor_UpdateThreshold(&instance->rightLineSensor, rightThreshold);
    }
}

void CheckSensors(void *context)
{
    LineDetector_t *instance = (LineDetector_t *)context;

    bool left = AnalogSensor_Check(&instance->leftLineSensor);
    bool right = AnalogSensor_Check(&instance->rightLineSensor);

    LineLocation_t location;

    if(left)
    {
        location = LL_Left;
    }
    else if(right)
    {
        location = LL_Right;
    }
    else
    {
        location = LL_NotDetected;
    }

    if(location != instance->lineLocation)
    {
        instance->lineLocation = location;
        Event_Publish(instance->lineDetectedEvent, (void *)&instance->lineLocation);
    }
}

void LineDetector_Init(LineDetector_t *instance, Event_t *lineDetectedEvent, Event_t *newSerialMessageEvent, TimerModule_t *timerModule)
{
    instance->lineDetectedEvent = lineDetectedEvent;

    EventSubscription_Init(&instance->newMessageSubscription, instance, ParseMessage);
    Event_Subscribe(newSerialMessageEvent, &instance->newMessageSubscription);

    AnalogSensor_Init(&instance->leftLineSensor, LINE_SENSOR_LEFT);
    AnalogSensor_Init(&instance->rightLineSensor, LINE_SENSOR_RIGHT);

    Timer_AddPeriodic(timerModule, &instance->timer, 10, instance, CheckSensors);
}
