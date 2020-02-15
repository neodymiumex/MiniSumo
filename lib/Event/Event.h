#ifndef _EVENT_H_
#define _EVENT_H_

#include "LinkedList.h"
#include "EventSubscription.h"

typedef struct
{
    LinkedList_t subscriptions;
} Event_t;

void Event_Init(Event_t *instance);
void Event_Publish(Event_t *instance, const void *args);
void Event_Subscribe(Event_t *instance, EventSubscription_t *subscription);
void Event_Unsubscribe(Event_t *instance, EventSubscription_t *subscription);

#endif
