#ifndef _COMBATCONTROLLER_H_
#define _COMBATCONTROLLER_H_

#include <stdint.h>
#include "Event.h"
#include "StateMachine.h"
#include "Timer.h"
#include "EnemyDetector.h"
#include "LineDetector.h"

#define START_COMBAT_STR "Go"
#define HALT_COMBAT_STR "Halt"

typedef struct
{
    EventSubscription_t lineDetectionSubscription;
    EventSubscription_t enemyDetectionSubscription;
    EventSubscription_t newSerialMessageSubscription;

    Event_t *motorRequestEvent;

    StateMachine_t stateMachine;
    TimerModule_t *timerModule;
    Timer_t countdownTimer;
    EnemyDirection_t enemyDirection;
    LineLocation_t lineLocation;
} CombatController_t;

void CombatController_Init(CombatController_t *instance,
                            Event_t *newSerialMessageEvent,
                            Event_t *lineDetectionEvent,
                            Event_t *enemyDetectionEvent,
                            Event_t *motorRequestEvent,
                            TimerModule_t *timerModule);

#endif
