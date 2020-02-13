#include "EventSubscription.h"

void EventSubscription_Init(EventSubscription_t *instance, void *context, void *callback)
{
    instance->context = context;
    instance->callback = callback;
}

void EventSubscription_Publish(EventSubscription_t *instance, const void *args)
{
    instance->callback(instance->context, args);
}
