#include "DigitalSensor.h"

bool DigitalSensor_Check(DigitalSensor_t *instance)
{
    int sensorValue;

    sensorValue = digitalRead(instance->inputPin);

    return sensorValue;
}

void DigitalSensor_Init(DigitalSensor_t *instance, uint8_t inputPin)
{
    instance->inputPin = inputPin;
    pinMode(inputPin, INPUT)
}
