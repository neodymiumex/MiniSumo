#include <Arduino.h>
#include "SerialReader.h"
#include "MotorController.h"
#include "EnemyDetector.h"
#include "LineDetector.h"
#include "SerialLogger.h"
#include "Timer.h"
#include "CombatController.h"

typedef struct
{
  Event_t newSerialMessageEvent;
  Event_t enemyDetectedEvent;
  Event_t lineDetectedEvent;
  Event_t logEvent;
  Event_t motorRequestEvent;
  SerialReader_t serialReader;
  MotorController_t motorController;
  EnemyDetector_t enemyDetector;
  LineDetector_t lineDetector;
  SerialLogger_t serialLogger;
  TimerModule_t timerModule;
  CombatController_t combatController;
} Vishnu_t;

Vishnu_t vishnu;

void setup()
{
  Event_Init(&vishnu.newSerialMessageEvent);
  Event_Init(&vishnu.enemyDetectedEvent);
  Event_Init(&vishnu.lineDetectedEvent);
  Event_Init(&vishnu.logEvent);
  Event_Init(&vishnu.motorRequestEvent);

  SerialReader_Init(&vishnu.serialReader, &vishnu.newSerialMessageEvent);
  MotorController_Init(&vishnu.motorController, &vishnu.motorRequestEvent, &vishnu.newSerialMessageEvent);
  EnemyDetector_Init(&vishnu.enemyDetector, &vishnu.enemyDetectedEvent, &vishnu.timerModule);
  LineDetector_Init(&vishnu.lineDetector, &vishnu.lineDetectedEvent, &vishnu.newSerialMessageEvent, &vishnu.timerModule);
  SerialLogger_Init(&vishnu.serialLogger, &vishnu.logEvent);
  TimerModule_Init(&vishnu.timerModule);

  Serial1.begin(9600);
  Serial1.println("<Vishnu is ready>");
}

void loop()
{
  SerialReader_ReadLine(&vishnu.serialReader);
  TimerModule_Run(&vishnu.timerModule);
}