#include "SerialReader.h"

static void AnnounceNewMessage(SerialReader_t *instance)
{
    Event_Publish(instance->newMessageNotifier, instance->buffer);
}

void SerialReader_ReadLine(SerialReader_t *instance)
{
    char rc;
    instance->messageInProgress = true;

    while (Hardware_Serial1DataAvailable(instance->hardware) > 0 && instance->messageInProgress == true)
    {
        rc = Hardware_ReadCharFromSerial1(instance->hardware);

        if (rc != END_OF_LINE_CHARACTER)
        {
            instance->buffer[instance->bufferIndex] = rc;
            instance->bufferIndex++;

            if (instance->bufferIndex >= MAX_SERIAL_BUFFER_SIZE)
            {
                instance->bufferIndex = MAX_SERIAL_BUFFER_SIZE - 1;
            }
        }
        else
        {
            instance->buffer[instance->bufferIndex] = '\0'; // terminate the string
            instance->bufferIndex = 0;
            instance->messageInProgress = false;
            AnnounceNewMessage(instance);
        }
    }
}

void SerialReader_Init(SerialReader_t *instance, Event_t *newMessageNotifier, I_Hardware_t *hardware)
{
    instance->hardware = hardware;
    instance->bufferIndex = 0;
    instance->messageInProgress = false;
    instance->newMessageNotifier = newMessageNotifier;
}
