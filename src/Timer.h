#ifndef _TIMER_H_
#define _TIMER_H_

#include "Event.h"
#include <Arduino.h>

typedef struct
{
    LinkedList_t *node;
    unsigned long timeoutMs;
    unsigned long duration;
    bool active;
    bool periodic;
    void *context;
    void (*callback)(void *context);
} Timer_t;

typedef struct
{
    LinkedList_t *timers;
    unsigned long currentMs;
} TimerModule_t;

void TimerModule_Init(TimerModule_t *instance);
void TimerModule_Run(TimerModule_t *instance);
void Timer_AddSingle(TimerModule_t *instance, Timer_t *timer, unsigned long durationMs, void *context, void *callback);
void Timer_AddPeriodic(TimerModule_t *instance, Timer_t *timer, unsigned long durationMs, void *context, void *callback);
void Timer_Stop(TimerModule_t *instance, Timer_t *timer);

#endif
