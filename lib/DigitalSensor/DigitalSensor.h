#ifndef _DIGITALSENSOR_H_
#define _DIGITALSENSOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "Event.h"
#include "I_Hardware.h"

typedef struct
{
    I_Hardware_t *hardware;
    uint8_t inputPin;
} DigitalSensor_t;

void DigitalSensor_Init(DigitalSensor_t *instance, uint8_t inputPin, I_Hardware_t *hardware);
bool DigitalSensor_Check(DigitalSensor_t *instance);

#endif
