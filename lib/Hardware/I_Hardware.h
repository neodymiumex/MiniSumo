#ifndef _I_HARDWARE_H_
#define _I_HARDWARE_H_

#include <stdint.h>
#include <stdbool.h>

#define MOTOR_1_DIR 15
#define MOTOR_1_EN 14
#define MOTOR_2_DIR 16
#define MOTOR_2_EN 10

#define PRESENCE_SENSOR_LEFT 2
#define PRESENCE_SENSOR_FRONTLEFT 3
#define PRESENCE_SENSOR_FRONT 4
#define PRESENCE_SENSOR_FRONTRIGHT 5
#define PRESENCE_SENSOR_RIGHT 6

#define LINE_SENSOR_LEFT  8
#define LINE_SENSOR_RIGHT 9
#define SERIAL_BAUD_RATE 115200

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
    void (*WriteDataToSerial1)(I_Hardware_t *instance, char *message, int length);
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

#define Hardware_WriteDataToSerial1(instance, message, length) \
    (instance)->api->WriteDataToSerial1((instance), (message), (length))

#define Hardware_GetCurrentMs(instance) \
    (instance)->api->GetCurrentMs(instance)

#endif
