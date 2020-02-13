#ifndef _SERIALREADER_H_
#define _SERIALREADER_H_

#include <stdint.h>
#include "Event.h"

#define MAX_SERIAL_BUFFER_SIZE 100
#define END_OF_LINE_CHARACTER '\n'

typedef struct
{
    char buffer[MAX_SERIAL_BUFFER_SIZE];
    uint8_t bufferIndex;
    bool messageInProgress;
    Event_t *newMessageNotifier;
} SerialReader_t;

void SerialReader_Init(SerialReader_t *instance, Event_t *newMessageNotifier);
void SerialReader_ReadLine(SerialReader_t *instance);

#endif
