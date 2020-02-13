#include "SerialLogger.h"
#include <Arduino.h>

static void WriteLog(SerialLogger_t *instance, void *args)
{
    char *message = (char *)args;

    Serial1.Write(message);
}

void SerialLogger_Init(SerialLogger_t *instance, Event_t *logEvent)
{
    EventSubscription_Init(&instance->logEventSubscription, instance, WriteLog);
    Event_Subscribe(logEvent, &instance->logEventSubscription);
}
