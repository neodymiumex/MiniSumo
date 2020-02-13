#ifndef _EVENTSUBSCRIPTION_H_
#define _EVENTSUBSCRIPTION_H_

#include "LinkedList.h"

typedef struct
{
    LinkedListNode_t *node;
    void *context;
    void (*callback)(void *context, void *args);
} EventSubscription_t;

void EventSubscription_Publish(EventSubscription_t *instance, const void *args);
void EventSubscription_Init(EventSubscription_t *instance, void *context, void *callback);

#endif
