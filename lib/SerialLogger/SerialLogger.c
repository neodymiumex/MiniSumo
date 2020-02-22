#include "SerialLogger.h"
#include <string.h>

static void SendBuffer(void *context)
{
    SerialLogger_t *instance = (SerialLogger_t *)context;

    if(instance->bufferIndex > 0)
    {
        Hardware_WriteDataToSerial1(instance->hardware, instance->buffer, instance->bufferIndex);

        instance->bufferIndex = 0;
    }
}

static void AddToBuffer(SerialLogger_t *instance, char *message)
{
    int length = strlen(message);
    char *overflow = "\n\nOVERFLOW\n\n";
    int overflowLength = strlen(overflow);

    if((instance->bufferIndex + length + overflowLength) >= MAX_BUFFER_SIZE)
    {
        message = overflow;
        length = overflowLength;
    }

    memcpy(instance->buffer[instance->bufferIndex], message, length);
    instance->bufferIndex += length;
}

static void WriteLog(SerialLogger_t *instance, void *args)
{
    char *message = (char *)args;

    if(Timer_IsActive(instance->timerModule, &instance->timer))
    {
        AddToBuffer(instance, message);
    }
    else
    {
        Hardware_WriteLineToSerial1(instance->hardware, message);
        Timer_AddSingle(instance->timerModule, &instance->timer, 12, SendBuffer);
    }
}

void SerialLogger_Init(SerialLogger_t *instance, Event_t *logEvent, I_Hardware_t *hardware, TimerModule_t *timerModule)
{
    instance->hardware = hardware;
    instance->bufferIndex = 0;
    instance->timerModule = timerModule;

    EventSubscription_Init(&instance->logEventSubscription, instance, WriteLog);
    Event_Subscribe(logEvent, &instance->logEventSubscription);
}
