#include "SerialLogger.h"

static void WriteLog(SerialLogger_t *instance, void *args)
{
    char *message = (char *)args;

    Hardware_WriteLineToSerial1(instance->hardware, message);
}

void SerialLogger_Init(SerialLogger_t *instance, Event_t *logEvent, I_Hardware_t *hardware)
{
    instance->hardware = hardware;

    EventSubscription_Init(&instance->logEventSubscription, instance, WriteLog);
    Event_Subscribe(logEvent, &instance->logEventSubscription);
}
