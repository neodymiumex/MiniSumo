#ifndef _ANALOGSENSOR_H_
#define _ANALOGSENSOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "Event.h"
#include "I_Hardware.h"

typedef struct
{
    I_Hardware_t *hardware;
    Event_t *detectionEvent;
    uint8_t inputPin;
    float threshold;
    float filterWeight;
    float currentValue;
} AnalogSensor_t;

void AnalogSensor_Init(AnalogSensor_t *instance, uint8_t inputPin, I_Hardware_t *hardware);
bool AnalogSensor_Check(AnalogSensor_t *instance);
int AnalogSensor_GetRawInput(AnalogSensor_t *instance);
void AnalogSensor_UpdateThreshold(AnalogSensor_t *instance, int threshold);
void AnalogSensor_UpdateFilterWeight(AnalogSensor_t *instance, int weight);

#endif
