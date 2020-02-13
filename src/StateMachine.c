#include "StateMachine.h"

void StateMachine_Signal(StateMachine_t *instance, const Signal_t signal, const void *args)
{
    instance->currentState(instance, signal, args);
}

void StateMachine_Transition(StateMachine_t *instance, State_t targetState)
{
    StateMachine_Signal(instance, SM_EXIT, NULL);
    instance->currentState = targetState;
    StateMachine_Signal(instance, SM_ENTRY, NULL);
}

void StateMachine_Init(StateMachine_t *instance, State_t initialState)
{
    instance->currentState = initialState;
}
