#ifndef _HARDWAREAPI_H_
#define _HARDWAREAPI_H_

#include <stdint.h>
#include <stdbool.h>

#define MOTOR_1_DIR 2
#define MOTOR_1_EN 3
#define MOTOR_2_DIR 4
#define MOTOR_2_EN 5
#define SERIAL_BAUD_RATE 9600

struct I_Hardware_Api_t;

typedef struct
{
    const struct I_Hardware_Api_t *api;
} I_Hardware_t;

typedef struct I_Hardware_Api_t
{
    void (*SetPinModeToOutput)(I_Hardware_t *instance, uint8_t pin);
    void (*SetPinModeToInput)(I_Hardware_t *instance, uint8_t pin);
    void (*SetDigitalPin)(I_Hardware_t *instance, uint8_t pin, bool high);
    void (*SetAnalogPin)(I_Hardware_t *instance, uint8_t pin, int value);
    bool (*ReadDigitalPin)(I_Hardware_t *instance, uint8_t pin);
    int (*ReadAnalogPin)(I_Hardware_t *instance, uint8_t pin);
    void (*SetSerial1BaudRate)(I_Hardware_t *instance, unsigned long baudRate);
    bool (*Serial1DataAvailable)(I_Hardware_t *instance);
    char (*ReadCharFromSerial1)(I_Hardware_t *instance);
    void (*WriteLineToSerial1)(I_Hardware_t *instance, char *message);
    unsigned long (*GetCurrentMs)(I_Hardware_t *instance);
} I_Hardware_Api_t;

#define Hardware_SetPinModeToOutput(instance, pin) \
    (instance)->api->SetPinModeToOutput((instance), (pin))

#define Hardware_SetPinModeToInput(instance, pin) \
    (instance)->api->SetPinModeToInput((instance), (pin))

#define Hardware_SetDigitalPin(instance, pin, high) \
    (instance)->api->SetDigitalPin((instance), (pin), (high))

#define Hardware_SetAnalogPin(instance, pin, value) \
    (instance)->api->SetAnalogPin((instance), (pin), (value))

#define Hardware_ReadDigitalPin(instance, pin) \
    (instance)->api->ReadDigitalPin((instance), (pin))

#define Hardware_ReadAnalogPin(instance, pin) \
    (instance)->api->ReadAnalogPin((instance), (pin))

#define Hardware_SetSerial1BaudRate(instance, baudRate) \
    (instance)->api->SetSerial1BaudRate((instance), (baudRate))

#define Hardware_Serial1DataAvailable(instance) \
    (instance)->api->Serial1DataAvailable(instance)

#define Hardware_ReadCharFromSerial1(instance) \
    (instance)->api->ReadCharFromSerial1(instance)

#define Hardware_WriteLineToSerial1(instance, message) \
    (instance)->api->WriteLineToSerial1((instance), (message))

#define Hardware_GetCurrentMs(instance) \
    (instance)->api->GetCurrentMs(instance)

#endif
