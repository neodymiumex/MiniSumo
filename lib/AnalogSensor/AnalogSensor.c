#include "AnalogSensor.h"
#include <stdint.h>

// Filter using exponential smoothing.
// Older readings are given less weight the older they are
static int ReadAndFilter(AnalogSensor_t *instance)
{
    int newValue = Hardware_ReadAnalogPin(instance->hardware, instance->inputPin);

    instance->currentValue = (INT32_C(1) * 100 * instance->filterWeight * newValue + INT32_C(1) * (100 - instance->filterWeight) * instance->currentValue + 50)/(INT32_C(1) *100);

    return instance->currentValue;
}

bool AnalogSensor_Check(AnalogSensor_t *instance)
{
    int sensorValue;
    bool result = false;

    sensorValue = ReadAndFilter(instance->hardware);

    if(sensorValue >= instance->threshold)
    {
        result = true;
    }

    return result;
}

void AnalogSensor_UpdateFilterWeight(AnalogSensor_t *instance, int weight)
{
     instance->filterWeight = weight;
}

void AnalogSensor_UpdateThreshold(AnalogSensor_t *instance, int threshold)
{
    instance->threshold = threshold;
}

int AnalogSensor_GetRawInput(AnalogSensor_t *instance)
{
    return Hardware_ReadAnalogPin(instance->hardware, instance->inputPin);
}

void AnalogSensor_Init(AnalogSensor_t *instance, uint8_t inputPin, I_Hardware_t *hardware)
{
    instance->inputPin = inputPin;
    instance->hardware = hardware;
    instance->filterWeight = 50;
    instance->currentValue = Hardware_ReadAnalogPin(hardware, inputPin);
}
