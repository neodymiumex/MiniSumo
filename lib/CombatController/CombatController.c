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
    Signal_CountdownExpired,
    Signal_StepTimerExpired,
    Signal_ManeuverComplete
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

static void StepTimerExpired(void *context)
{
    CombatController_t *instance = (CombatController_t *)context;

    instance->stepIndex++;

    if(instance->stepIndex < ManeuverList[instance->currentManuever]->stepCount)
    {
        StartTimedStep(instance);
    }
    else
    {
        StateMachine_Signal(&instance->stateMachine, Signal_ManeuverComplete, NULL);
    }

}

static void StartStep(CombatController_t *instance, ManeuverStep_t step)
{
    RequestMotors(instance, StepsList[step].left, StepsList[step].right);
}

static void StartTimedStep(CombatController_t *instance)
{
    RequestMotors(
        instance,
        StepsList[ManeuverList[instance->currentManuever]->steps[instance->stepIndex].step].left,
        StepsList[ManeuverList[instance->currentManuever]->steps[instance->stepIndex].step].right);

    Timer_AddSingle(
        instance->timerModule,
        &instance->manueverTimer,
        ManeuverList[instance->currentManuever]->steps[instance->stepIndex].durationMs,
        instance,
        StepTimerExpired);
}

static void StartManeuver(CombatController_t *instance, ManeuverType_t type)
{
    Timer_Stop(instance->timerModule, &instance->manueverTimer);
    instance->currentManuever = type;
    instance->stepIndex = 0;
    StartTimedStep(instance);
}

static void StartTurnTowardsEnemy(CombatController_t *instance)
{
    if(instance->enemyDirection == ED_Left)
    {
        StartStep(instance, Step_SpinLeft);
    }
    else if(instance->enemyDirection == ED_FrontLeft)
    {
        StartStep(instance, Step_ForwardLeft);
    }
    else if(instance->enemyDirection == ED_Right)
    {
        StartStep(instance, Step_SpinRight);
    }
    else if(instance->enemyDirection == ED_FrontRight)
    {
        StartStep(instance, Step_ForwardRight);
    }
}

static void StartSearch(CombatController_t *instance)
{
    StartManeuver(instance, MT_Go);
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
static void State_BreakDeadlock(void *context, Signal_t signal, const void *args);
static void State_Stop(void *context, Signal_t signal, const void *args);

static void State_Idle(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            StartStep(instance, MS_Stop);
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
            if(instance->lineLocation == LL_NotDetected)
            {
                StartSearch(instance);
            }
            else
            {
                StartManeuver(instance, MT_Reverse);
            }
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
            StateMachine_Transition(&instance->stateMachine, State_Stop);
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
            if(instance->lineLocation == LL_NotDetected)
            {
                StartTurnTowardsEnemy(instance);
            }
            else
            {
                StartManeuver(instance, MT_Reverse);
            }
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

        case Signal_LineDetected:
            StateMachine_Transition(&instance->stateMachine, State_TurnAround);
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Stop);
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

        case Signal_LineDetected:
            StateMachine_Transition(&instance->stateMachine, State_TurnAround);
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Stop);
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
            if(instance->lineLocation == LL_Left)
            {
                StartManeuver(instance, MT_TurnAroundLeft);
            }
            else if(instance->lineLocation == LL_Right)
            {
                StartManeuver(instance, MT_TurnAroundRight);
            }
            break;

        case Signal_LineDetected:
            if(instance->lineLocation == LL_NotDetected)
            {
                StateMachine_Transition(&instance->stateMachine, State_Search);
            }
            else if(instance->lineLocation == LL_Left)
            {
                StartManeuver(instance, MT_Reverse);
            }
            break;

        case Signal_ManeuverComplete:
            StateMachine_Transition(&instance->stateMachine, State_Search);
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Stop);
            break;

        case SM_EXIT:
            break;
    }
}

static void State_BreakDeadlock(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            StartManeuver(instance, MT_BreakDeadlock);
            break;

        case Signal_ManeuverComplete:
            StateMachine_Transition(&instance->stateMachine, State_Search);
            break;

        case Signal_LineDetected:
            StateMachine_Transition(&instance->stateMachine, State_TurnAround);
            break;

        case Signal_Halt:
            StateMachine_Transition(&instance->stateMachine, State_Stop);
            break;

        case SM_EXIT:
            break;
    }
}

static void State_Stop(void *context, Signal_t signal, const void *args)
{
    CombatController_t *instance = (CombatController_t *)context;

    switch(signal)
    {
        case SM_ENTRY:
            StartManeuver(instance, MT_Brake);
            break;

        case Signal_ManeuverComplete:
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
