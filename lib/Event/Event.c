#include "Event.h"

void Event_Subscribe(Event_t *instance, EventSubscription_t *subscription)
{
    LinkedList_Push(instance->subscriptions, (LinkedListNode_t *)subscription);
}

void Event_Unsubscribe(Event_t *instance, EventSubscription_t *subscription)
{
    LinkedList_Remove(instance->subscriptions, (LinkedListNode_t *)subscription);
}

void Event_Publish(Event_t *instance, const void *args)
{
    LinkedList_ForEach(instance->subscriptions, EventSubscription_Publish, args);
}

void Event_Init(Event_t *instance)
{
    LinkedList_Init(instance->subscriptions);
}
