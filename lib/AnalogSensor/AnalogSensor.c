#include "AnalogSensor.h"

bool AnalogSensor_Check(AnalogSensor_t *instance)
{
    int sensorValue;
    bool result = false;

    sensorValue = Hardware_ReadAnalogPin(instance->hardware, instance->inputPin);

    if(sensorValue > instance->threshold)
    {
        result = true;
    }

    return result;
}

void AnalogSensor_UpdateThreshold(AnalogSensor_t *instance, int threshold)
{
    instance->threshold = threshold;
}

void AnalogSensor_Init(AnalogSensor_t *instance, uint8_t inputPin, I_Hardware_t *hardware)
{
    instance->inputPin = inputPin;
}
