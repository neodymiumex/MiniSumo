#ifndef _STATEMACHINE_H_
#define _STATEMACHINE_H_

#include <stdint.h>

enum
{
    SM_EXIT,
    SM_ENTRY,
    SM_SIGNAL_START
};

struct StateMachine_t;

typedef uint8_t Signal_t;
typedef void (*State_t)(void *context, const Signal_t signal, const void *args);

typedef struct StateMachine_t
{
    State_t currentState;
} StateMachine_t;

void StateMachine_Transition(StateMachine_t *instance, State_t targetState);
void StateMachine_Signal(StateMachine_t *instance, const Signal_t signal, const void *args);
void StateMachine_Init(StateMachine_t *instance, State_t initialState);

#endif
