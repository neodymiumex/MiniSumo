#ifndef _SERIALLOGGER_H_
#define _SERIALLOGGER_H_

#include <stdint.h>
#include "Event.h"

typedef struct
{
    EventSubscription_t logEventSubscription;
} SerialLogger_t;

void SerialLogger_Init(SerialLogger_t *instance, Event_t *logEvent);

#endif
