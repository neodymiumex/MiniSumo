#include "CombatController.h"
#include "LineDetector.h"
#include "EnemyDetector.h"

enum
{
    Signal_Start = SM_SIGNAL_START,
    Signal_Halt,
    Signal_LineDetected,
    Signal_EnemyDetected,
    Signal_CountdownExpired
};

static void OnEnemyDetection(void *context, void *args)
{
    CombatController_t *instance = (CombatController_t *)context;
    EnemyDirection_t *direction = (EnemyDirection_t *)args;

    StateMachine_Signal(&instance->stateMachine, Signal_EnemyDetected, direction);
}

static void OnLineDetection(void *context, void *args)
{
    CombatController_t *instance = (CombatController_t *)context;
    LineLocation_t *location = (LineLocation_t *)args;

    StateMachine_Signal(&instance->stateMachine, Signal_LineDetected, location);
}

static void ParseMessage(void *context, void *args)
{
    CombatController_t *instance = (CombatController_t *)context;
    char *message = (char *)args;
    char *startStr = START_COMBAT_STR;
    char *haltStr = HALT_COMBAT_STR;

    if(strstr(message, &startStr))
    {
        StateMachine_Signal(&instance->stateMachine, Signal_Start, NULL);
    }
    else if(strstr(message, &haltStr))
    {
        StateMachine_Signal(&instance->stateMachine, Signal_Halt, NULL);
    }
}

static void CountdownElapsed(void *context, void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    StateMachine_Signal(&instance->stateMachine, Signal_CountdownExpired, NULL);
}

State_t State_Idle(void *context, Signal_t signal, const void *args);
State_t State_Countdown(void *context, Signal_t signal, const void *args);
State_t State_Search(void *context, Signal_t signal, const void *args);
State_t State_Attack(void *context, Signal_t signal, const void *args);
State_t State_TurnAround(void *context, Signal_t signal, const void *args);
State_t State_FollowEnemy(void *context, Signal_t signal, const void *args);

State_t State_Idle(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            break;

        case Signal_Start:
            StateMachine_Transition(&instance->stateMachine, State_Countdown);
            break;

        case Signal_EnemyDetected:
            instance->enemyDirection = (const EnemyDirection_t *)args;
            break;

        case SM_EXIT:
            break;
    }
}
State_t State_Countdown(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            Timer_AddSingle(instance->timerModule, &instance->countdownTimer, 5000, instance, CountdownElapsed);
            break;

        case Signal_CountdownExpired:
            if(instance->enemyDirection == ED_NotDetected)
            {
                StateMachine_Transition(&instance->stateMachine, State_Search);
            }
            else
            {
                StateMachine_Transition(&instance->stateMachine, State_FollowEnemy);
            }

            break;

        case Signal_EnemyDetected:
            instance->enemyDirection = (const EnemyDirection_t *)args;
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Idle);
            break;

        case SM_EXIT:
            Timer_Stop(instance->timerModule, &instance->countdownTimer);
            break;
    }
}

State_t State_Search(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Idle);
            break;

        case SM_EXIT:
            break;
    }
}

State_t State_FollowEnemy(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Idle);
            break;

        case SM_EXIT:
            break;
    }
}

State_t State_Attack(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Idle);
            break;

        case SM_EXIT:
            break;
    }
}

State_t State_TurnAround(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Idle);
            break;

        case SM_EXIT:
            break;
    }
}

void CombatController_Init(CombatController_t *instance, Event_t *newSerialMessageEvent, Event_t *lineDetectionEvent, Event_t *enemyDetectionEvent, TimerModule_t *timerModule)
{
    instance->timerModule = timerModule;

    EventSubscription_Init(&instance->enemyDetectionSubscription, instance, OnEnemyDetection);
    Event_Subscribe(enemyDetectionEvent, &instance->enemyDetectionSubscription);

    EventSubscription_Init(&instance->lineDetectionSubscription, instance, OnLineDetection);
    Event_Subscribe(lineDetectionEvent, &instance->lineDetectionSubscription);

    EventSubscription_Init(&instance->newSerialMessageSubscription, instance, ParseMessage);
    Event_Subscribe(newSerialMessageEvent, &instance->newSerialMessageSubscription);

    StateMachine_Init(&instance->stateMachine, State_Idle);
}
