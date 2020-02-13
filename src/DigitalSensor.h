#ifndef _DIGITALSENSOR_H_
#define _DIGITALSENSOR_H_

#include <stdint.h>
#include "Event.h"
#include <Arduino.h>

typedef struct
{
    uint8_t inputPin;
} DigitalSensor_t;

void DigitalSensor_Init(DigitalSensor_t *instance, uint8_t inputPin);
bool DigitalSensor_Check(DigitalSensor_t *instance);

#endif
