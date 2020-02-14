#ifndef _SERIALLOGGER_H_
#define _SERIALLOGGER_H_

#include <stdint.h>
#include "Event.h"
#include "I_Hardware.h"

typedef struct
{
    I_Hardware_t *hardware;
    EventSubscription_t logEventSubscription;
} SerialLogger_t;

void SerialLogger_Init(SerialLogger_t *instance, Event_t *logEvent, I_Hardware_t *hardware);

#endif
