#include "DigitalSensor.h"

bool DigitalSensor_Check(DigitalSensor_t *instance)
{
    return Hardware_ReadDigitalPin(instance->hardware, instance->inputPin);
}

void DigitalSensor_Init(DigitalSensor_t *instance, uint8_t inputPin, I_Hardware_t *hardware)
{
    instance->hardware = hardware;
    instance->inputPin = inputPin;
    Hardware_SetPinModeToInput(instance->hardware, inputPin);
}
