#include "CombatController.h"
#include "MotorController.h"
#include <stddef.h>
#include <string.h>

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

    instance->enemyDirection = *direction;
    StateMachine_Signal(&instance->stateMachine, Signal_EnemyDetected, NULL);
}

static void OnLineDetection(void *context, void *args)
{
    CombatController_t *instance = (CombatController_t *)context;
    LineLocation_t *location = (LineLocation_t *)args;

    instance->lineLocation = *location;
    StateMachine_Signal(&instance->stateMachine, Signal_LineDetected, NULL);
}

static void ParseMessage(void *context, void *args)
{
    CombatController_t *instance = (CombatController_t *)context;
    char *message = (char *)args;
    const char *startStr = START_COMBAT_STR;
    const char *haltStr = HALT_COMBAT_STR;

    if(strstr(message, startStr))
    {
        StateMachine_Signal(&instance->stateMachine, Signal_Start, NULL);
    }
    else if(strstr(message, haltStr))
    {
        StateMachine_Signal(&instance->stateMachine, Signal_Halt, NULL);
    }
}

static void RequestMotors(CombatController_t *instance, MotorSpeed_t leftMotor, MotorSpeed_t rightMotor)
{
    MotorRequest_t request;
    request.left = leftMotor;
    request.right = rightMotor;

    Event_Publish(instance->motorRequestEvent, &request);
}

static void StartTurnTowardsEnemy(CombatController_t *instance)
{
    if((instance->enemyDirection == ED_Left) || (instance->enemyDirection == ED_FrontLeft))
    {
        RequestMotors(instance, MS_Stop, MS_GoSlow);
    }
    else if((instance->enemyDirection == ED_Right) || (instance->enemyDirection == ED_FrontRight))
    {
        RequestMotors(instance, MS_GoSlow, MS_Stop);
    }
}

static void StartAttack(CombatController_t *instance)
{
    RequestMotors(instance, MS_GoFast, MS_GoFast);
}

static void StartTurnAround(CombatController_t *instance)
{

}

static void Stop(CombatController_t *instance)
{
    RequestMotors(instance, MS_Stop, MS_Stop);
}

static void StartSearch(CombatController_t *instance)
{

}

static void CountdownElapsed(void *context, void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    StateMachine_Signal(&instance->stateMachine, Signal_CountdownExpired, NULL);
}

static void State_Idle(void *context, Signal_t signal, const void *args);
static void State_Countdown(void *context, Signal_t signal, const void *args);
static void State_Search(void *context, Signal_t signal, const void *args);
static void State_Attack(void *context, Signal_t signal, const void *args);
static void State_TurnAround(void *context, Signal_t signal, const void *args);
static void State_FollowEnemy(void *context, Signal_t signal, const void *args);

static void State_Idle(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            Stop(instance);
            break;

        case Signal_Start:
            StateMachine_Transition(&instance->stateMachine, State_Countdown);
            break;

        case Signal_EnemyDetected:
            instance->enemyDirection = *(const EnemyDirection_t *)args;
            break;

        case SM_EXIT:
            break;
    }
}

static void State_Countdown(void *context, Signal_t signal, const void *args)
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
            else if(instance->enemyDirection == ED_Front)
            {
                StateMachine_Transition(&instance->stateMachine, State_Attack);
            }
            else
            {
                StateMachine_Transition(&instance->stateMachine, State_FollowEnemy);
            }
            break;

        case Signal_EnemyDetected:
            instance->enemyDirection = *(const EnemyDirection_t *)args;
            break;

        case SM_EXIT:
            Timer_Stop(instance->timerModule, &instance->countdownTimer);
            break;
    }
}

static void State_Search(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            StartSearch(instance);
            break;

        case Signal_EnemyDetected:
            if(instance->enemyDirection == ED_Front)
            {
                StateMachine_Transition(&instance->stateMachine, State_Attack);
            }
            else if(instance->enemyDirection != ED_NotDetected)
            {
                StateMachine_Transition(&instance->stateMachine, State_FollowEnemy);
            }
            break;

        case Signal_LineDetected:
            StateMachine_Transition(&instance->stateMachine, State_TurnAround);
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Idle);
            break;

        case SM_EXIT:
            break;
    }
}

static void State_FollowEnemy(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            StartTurnTowardsEnemy(instance);
            break;

        case Signal_EnemyDetected:
            if(instance->enemyDirection == ED_Front)
            {
                StateMachine_Transition(&instance->stateMachine, State_Attack);
            }
            else if(instance->enemyDirection == ED_NotDetected)
            {
                StateMachine_Transition(&instance->stateMachine, State_Search);
            }
            else
            {
                StartTurnTowardsEnemy(instance);
            }
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Idle);
            break;

        case SM_EXIT:
            break;
    }
}

static void State_Attack(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            StartAttack(instance);
            break;

        case Signal_EnemyDetected:
            if(instance->enemyDirection == ED_NotDetected)
            {
                StateMachine_Transition(&instance->stateMachine, State_Search);
            }
            else if(instance->enemyDirection != ED_Front)
            {
                StateMachine_Transition(&instance->stateMachine, State_FollowEnemy);
            }
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Idle);
            break;

        case SM_EXIT:
            break;
    }
}

static void State_TurnAround(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            StartTurnAround(instance);
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Idle);
            break;

        case SM_EXIT:
            break;
    }
}

void CombatController_Init(CombatController_t *instance, Event_t *newSerialMessageEvent, Event_t *lineDetectionEvent, Event_t *enemyDetectionEvent, Event_t *motorRequestEvent, TimerModule_t *timerModule)
{
    instance->timerModule = timerModule;
    instance->motorRequestEvent = motorRequestEvent;

    EventSubscription_Init(&instance->enemyDetectionSubscription, instance, OnEnemyDetection);
    Event_Subscribe(enemyDetectionEvent, &instance->enemyDetectionSubscription);

    EventSubscription_Init(&instance->lineDetectionSubscription, instance, OnLineDetection);
    Event_Subscribe(lineDetectionEvent, &instance->lineDetectionSubscription);

    EventSubscription_Init(&instance->newSerialMessageSubscription, instance, ParseMessage);
    Event_Subscribe(newSerialMessageEvent, &instance->newSerialMessageSubscription);

    StateMachine_Init(&instance->stateMachine, State_Idle);
}
