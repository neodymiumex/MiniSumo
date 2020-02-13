#ifndef _ANALOGSENSOR_H_
#define _ANALOGSENSOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "Event.h"

typedef struct
{
    Event_t *detectionEvent;
    uint8_t inputPin;
    int threshold;
} AnalogSensor_t;

void AnalogSensor_Init(AnalogSensor_t *instance, uint8_t inputPin);
bool AnalogSensor_Check(AnalogSensor_t *instance);
void AnalogSensor_UpdateThreshold(AnalogSensor_t *instance, int threshold);

#endif
