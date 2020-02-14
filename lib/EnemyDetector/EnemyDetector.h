#ifndef _ENEMYDETECTOR_H_
#define _ENEMYDETECTOR_H_

#include "DigitalSensor.h"
#include "Event.h"
#include "Timer.h"
#include "Hardware.h"

typedef enum
{
    ED_NotDetected,
    ED_Left,
    ED_FrontLeft,
    ED_Front,
    ED_FrontRight,
    ED_Right
} EnemyDirection_t;


typedef struct
{
    DigitalSensor_t leftSensor;
    DigitalSensor_t rightSensor;
    DigitalSensor_t frontSensor;
    DigitalSensor_t frontLeftSensor;
    DigitalSensor_t frontRightSensor;
    Event_t *enemyDetectedEvent;
    EnemyDirection_t enemyDirection;
    Timer_t timer;
} EnemyDetector_t;

void EnemyDetector_Init(EnemyDetector_t *instance, Event_t *enemyDetectedEvent, TimerModule_t *timerModule, I_Hardware_t *hardware);

#endif
