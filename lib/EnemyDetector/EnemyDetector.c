#include "EnemyDetector.h"
#include <string.h>

static void CheckSensors(void *context)
{
  EnemyDetector_t *instance = (EnemyDetector_t *)context;

  bool left = DigitalSensor_Check(&instance->leftSensor);
  bool frontLeft = DigitalSensor_Check(&instance->frontLeftSensor);
  bool front = DigitalSensor_Check(&instance->frontSensor);
  bool frontRight = DigitalSensor_Check(&instance->frontRightSensor);
  bool right = DigitalSensor_Check(&instance->rightSensor);

  EnemyDirection_t direction;

  if(left)
  {
    direction = ED_Left;
  }
  else if (right)
  {
    direction = ED_Right;
  }
  else if (frontLeft)
  {
    direction = ED_FrontLeft;
  }
  else if (frontRight)
  {
    direction = ED_FrontRight;
  }
  else if (front)
  {
    direction = ED_Front;
  }
  else
  {
    direction = ED_NotDetected;
  }

  if(direction != instance->enemyDirection)
  {
    instance->enemyDirection = direction;
    Event_Publish(instance->enemyDetectedEvent, (void *)&instance->enemyDirection);
  }
}

void EnemyDetector_Init(EnemyDetector_t *instance, Event_t *enemyDetectedEvent, TimerModule_t *timerModule, I_Hardware_t *hardware)
{
  instance->enemyDetectedEvent = enemyDetectedEvent;

  DigitalSensor_Init(&instance->leftSensor, PRESENCE_SENSOR_LEFT, hardware);
  DigitalSensor_Init(&instance->frontLeftSensor, PRESENCE_SENSOR_FRONTLEFT, hardware);
  DigitalSensor_Init(&instance->frontSensor, PRESENCE_SENSOR_FRONT, hardware);
  DigitalSensor_Init(&instance->frontRightSensor, PRESENCE_SENSOR_FRONTRIGHT, hardware);
  DigitalSensor_Init(&instance->rightSensor, PRESENCE_SENSOR_RIGHT, hardware);

  Timer_AddPeriodic(timerModule, &instance->timer, 10, instance, CheckSensors);
}
