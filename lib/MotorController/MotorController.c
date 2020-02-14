#include <stdint.h>
#include <stdlib.h>
#include "MotorController.h"

static void UpdateMotorSpeeds(MotorController_t *instance, MotorSpeed_t leftMotor, MotorSpeed_t rightMotor)
{
    Hardware_SetAnalogPin(instance->hardware, MOTOR_1_EN, leftMotor);
    Hardware_SetAnalogPin(instance->hardware, MOTOR_2_EN, rightMotor);
}

static void HandleRequest(void *context, void *args)
{
    MotorController_t *instance = (MotorController_t *)context;
    MotorRequest_t *request = (MotorRequest_t *)args;

    UpdateMotorSpeeds(instance, request->left, request->right);
}

static void ParseMessage(void *context, void *args)
{
    MotorController_t *instance = (MotorController_t *)context;
    char *message = (char *)args;
    char moveChar = MOVE_CHAR;
    char *settings;

    if(settings = strstr(message, &moveChar))
    {
        char *substr;
        char xChar = X_CHAR;
        char yChar = Y_CHAR;
        int x = 0;
        int y = 0;

        if(substr = strstr(settings, &xChar))
        {
            x = atoi(substr + 2);
        }

        if(substr = strstr(settings, &yChar))
        {
            y = atoi(substr + 2);
        }

        float   pivotLimit = PIVOT;
        float   premixLeft;
        float   premixRight;
        int8_t  pivotSpeed;
        float   scale;
        int8_t  mixedLeft;
        int8_t  mixedRight;

        if (y >= 0)
        {
            premixLeft = (x >= 0) ? 127.0 : (127.0 + x);
            premixRight = (x >= 0) ? (127.0 - x) : 127.0;
        }
        else
        {
            premixLeft = (x >= 0) ? (127.0 - x) : 127.0;
            premixRight = (x >= 0) ? 127.0 : (127.0 + x);
        }

        premixLeft = premixLeft * y / 128.0;
        premixRight = premixRight * y / 128.0;

        pivotSpeed = x;
        scale = (abs(y) > pivotLimit) ? 0.0 : (1.0 - abs(y) / pivotLimit);

        mixedLeft = (1.0 - scale) * premixLeft + scale * (pivotSpeed);
        mixedRight = (1.0 - scale) * premixRight + scale * (-pivotSpeed);

        UpdateMotorSpeeds(instance, mixedLeft, mixedRight);
    }
}

void MotorController_Init(MotorController_t *instance, Event_t *motorRequestEvent, Event_t *newSerialMessageEvent, I_Hardware_t *hardware)
{
    instance->hardware = hardware;

    EventSubscription_Init(&instance->newMessageSubscription, instance, ParseMessage);
    Event_Subscribe(newSerialMessageEvent, &instance->newMessageSubscription);

    EventSubscription_Init(&instance->motorRequestSubscription, instance, HandleRequest);
    Event_Subscribe(motorRequestEvent, &instance->motorRequestSubscription);
}
