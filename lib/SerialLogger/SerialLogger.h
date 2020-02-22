#ifndef _SERIALLOGGER_H_
#define _SERIALLOGGER_H_

#include <stdint.h>
#include "Event.h"
#include "I_Hardware.h"
#include "Timer.h"

#define MAX_BUFFER_SIZE 200
typedef struct
{
    I_Hardware_t *hardware;
    EventSubscription_t logEventSubscription;
    char buffer[MAX_BUFFER_SIZE];
    uint8_t bufferIndex;
    TimerModule_t *timerModule;
    Timer_t timer;
} SerialLogger_t;

void SerialLogger_Init(SerialLogger_t *instance, Event_t *logEvent, I_Hardware_t *hardware, TimerModule_t *timerModule);

#endif
