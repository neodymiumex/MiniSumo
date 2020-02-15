
#include "Hardware_Mock.h"
#include <string.h>

void Hardware_Mock_ElapseTime(Hardware_Mock_t *instance, unsigned long timeElapsed)
{
    *instance->config->currentMs += timeElapsed;
}

void Hardware_Mock_SetSerial1DataAvailable(Hardware_Mock_t *instance, bool available)
{
    *instance->config->serialAvailable = available;
}

void SetPinModeToOutput(I_Hardware_t *instance, uint8_t pin)
{
    Hardware_Mock_t *hardware  = (Hardware_Mock_t *)instance;
    *(hardware->config->pinMode + pin) = 1;
}

void SetPinModeToInput(I_Hardware_t *instance, uint8_t pin)
{
    Hardware_Mock_t *hardware  = (Hardware_Mock_t *)instance;
    *(hardware->config->pinMode + pin) = 2;
}

void SetDigitalPin(I_Hardware_t *instance, uint8_t pin, bool high)
{
    Hardware_Mock_t *hardware  = (Hardware_Mock_t *)instance;
    *(hardware->config->digitalPinValue + pin) = high;
}

void SetAnalogPin(I_Hardware_t *instance, uint8_t pin, int value)
{
    Hardware_Mock_t *hardware  = (Hardware_Mock_t *)instance;
    *(hardware->config->analogPinValue + pin) = value;
}

bool ReadDigitalPin(I_Hardware_t *instance, uint8_t pin)
{
    Hardware_Mock_t *hardware  = (Hardware_Mock_t *)instance;
    return *(hardware->config->digitalPinValue + pin);
}

int ReadAnalogPin(I_Hardware_t *instance, uint8_t pin)
{
    Hardware_Mock_t *hardware  = (Hardware_Mock_t *)instance;
    return *(hardware->config->analogPinValue + pin);
}

void SetSerial1BaudRate(I_Hardware_t *instance, unsigned long baudRate)
{

}

bool Serial1DataAvailable(I_Hardware_t *instance)
{
    Hardware_Mock_t *hardware  = (Hardware_Mock_t *)instance;
    return *hardware->config->serialAvailable;
}

char ReadCharFromSerial1(I_Hardware_t *instance)
{
    Hardware_Mock_t *hardware  = (Hardware_Mock_t *)instance;
    char rc;
    rc = *hardware->config->serialChar;

    hardware->config->serialChar++;
    hardware->strLen--;

    if(hardware->strLen == 0)
    {
        Hardware_Mock_SetSerial1DataAvailable(hardware, false);
    }

    return rc;
}

void WriteLineToSerial1(I_Hardware_t *instance, char *message)
{
    Hardware_Mock_t *hardware  = (Hardware_Mock_t *)instance;
    hardware->strLen = strlen(message);
    memcpy(hardware->config->serialChar, message, hardware->strLen);
    Hardware_Mock_SetSerial1DataAvailable(hardware, true);
}

unsigned long GetCurrentMs(I_Hardware_t *instance)
{
    Hardware_Mock_t *hardware  = (Hardware_Mock_t *)instance;
    return *hardware->config->currentMs;
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

void Hardware_Mock_Init(Hardware_Mock_t *instance, Hardware_Mock_Config_t *config)
{
    instance->interface.api = &api;
    instance->config = config;

    *instance->config->digitalPinValue = 0;
    *instance->config->analogPinValue = 0;
    *instance->config->serialAvailable = false;
    *instance->config->serialChar = '\0';
    *instance->config->currentMs = 0;
}
