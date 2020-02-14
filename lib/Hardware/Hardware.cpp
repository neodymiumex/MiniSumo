#ifndef UNIT_TEST

#include "Hardware.h"
#include <Arduino.h>

void SetPinModeToOutput(I_Hardware_t *instance, uint8_t pin)
{
    pinMode(pin, OUTPUT);
}

void SetPinModeToInput(I_Hardware_t *instance, uint8_t pin)
{
    pinMode(pin, INPUT);
}

void SetDigitalPin(I_Hardware_t *instance, uint8_t pin, bool high)
{
    digitalWrite(pin, high);
}

void SetAnalogPin(I_Hardware_t *instance, uint8_t pin, int value)
{
    analogWrite(pin, value);
}

bool ReadDigitalPin(I_Hardware_t *instance, uint8_t pin)
{
    return digitalRead(pin);
}

int ReadAnalogPin(I_Hardware_t *instance, uint8_t pin)
{
    return analogRead(pin);
}

void SetSerial1BaudRate(I_Hardware_t *instance, unsigned long baudRate)
{
    Serial1.begin(baudRate);
}

bool Serial1DataAvailable(I_Hardware_t *instance)
{
    return Serial1.available();
}

char ReadCharFromSerial1(I_Hardware_t *instance)
{
    return Serial1.read();
}

void WriteLineToSerial1(I_Hardware_t *instance, char *message)
{
    Serial1.println(message);
}

unsigned long GetCurrentMs(I_Hardware_t *instance)
{
    return millis();
}

static const I_Hardware_Api_t api =
{
    SetPinModeToOutput,
    SetPinModeToInput,
    SetDigitalPin,
    SetAnalogPin,
    ReadDigitalPin,
    ReadAnalogPin,
    SetSerial1BaudRate,
    Serial1DataAvailable,
    ReadCharFromSerial1,
    WriteLineToSerial1,
    GetCurrentMs
};

void Hardware_Init(Hardware_t *instance)
{
    instance->interface.api = &api;

    SetPinModeToOutput(&instance->interface, MOTOR_1_EN);
    SetPinModeToOutput(&instance->interface, MOTOR_1_DIR);
    SetDigitalPin(&instance->interface, MOTOR_1_DIR, true);

    SetPinModeToOutput(&instance->interface, MOTOR_2_EN);
    SetPinModeToOutput(&instance->interface, MOTOR_2_DIR);
    SetDigitalPin(&instance->interface, MOTOR_2_DIR, true);

    SetSerial1BaudRate(&instance->interface, SERIAL_BAUD_RATE);

}

#endif
