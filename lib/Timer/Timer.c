#include "Timer.h"

void TimerModule_Run(TimerModule_t *instance)
{
    unsigned long currentMs = Hardware_GetCurrentMs(instance->hardware);

    if(currentMs > instance->currentMs)
    {
        Timer_t *current = (Timer_t *)instance->timers.head;
        instance->currentMs = currentMs;

        while(current)
        {
            if(currentMs >= current->timeoutMs)
            {
                current->callback(current->context);

                if(current->periodic)
                {
                    current->timeoutMs = currentMs + current->duration;
                }
                else
                {
                    current->active = false;
                    LinkedList_Remove(&instance->timers, (LinkedListNode_t *)current);
                }
            }

            current = (Timer_t *)((LinkedListNode_t *)current)->next;
        }
    }
}

void Timer_Stop(TimerModule_t *instance, Timer_t *timer)
{
    LinkedList_Remove(&instance->timers, (LinkedListNode_t *)timer);
}

void Timer_AddSingle(TimerModule_t *instance, Timer_t *timer, unsigned long durationMs, void *context, void *callback)
{
    Timer_Stop(instance, timer);

    timer->timeoutMs = instance->currentMs + durationMs;
    timer->duration = durationMs;
    timer->active = true;
    timer->periodic = false;
    timer->context = context;
    timer->callback = callback;
    LinkedList_Push(&instance->timers, (LinkedListNode_t *)timer);
}

void Timer_AddPeriodic(TimerModule_t *instance, Timer_t *timer, unsigned long durationMs, void *context, void *callback)
{
    Timer_Stop(instance, timer);

    timer->timeoutMs = instance->currentMs + durationMs;
    timer->duration = durationMs;
    timer->active = true;
    timer->periodic = true;
    timer->context = context;
    timer->callback = callback;
    LinkedList_Push(&instance->timers, (LinkedListNode_t *)timer);
}

void TimerModule_Init(TimerModule_t *instance, I_Hardware_t *hardware)
{
    instance->currentMs = 0;
    instance->hardware = hardware;
    LinkedList_Init(&instance->timers);
}
