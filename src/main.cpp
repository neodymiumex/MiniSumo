#include <Arduino.h>

extern "C"
{
#include "SerialReader.h"
#include "MotorController.h"
#include "EnemyDetector.h"
#include "LineDetector.h"
#include "SerialLogger.h"
#include "Timer.h"
#include "CombatController.h"
#include "Hardware.h"
}

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
  Hardware_t hardware;
} Vishnu_t;

Vishnu_t vishnu;

void setup()
{
  Event_Init(&vishnu.newSerialMessageEvent);
  Event_Init(&vishnu.enemyDetectedEvent);
  Event_Init(&vishnu.lineDetectedEvent);
  Event_Init(&vishnu.logEvent);
  Event_Init(&vishnu.motorRequestEvent);

  Hardware_Init(&vishnu.hardware);

  SerialReader_Init(&vishnu.serialReader, &vishnu.newSerialMessageEvent, &vishnu.hardware.interface);
  MotorController_Init(&vishnu.motorController, &vishnu.motorRequestEvent, &vishnu.newSerialMessageEvent, &vishnu.hardware.interface);
  LineDetector_Init(&vishnu.lineDetector, &vishnu.lineDetectedEvent, &vishnu.newSerialMessageEvent, &vishnu.timerModule, &vishnu.hardware.interface);
  SerialLogger_Init(&vishnu.serialLogger, &vishnu.logEvent, &vishnu.hardware.interface);
  TimerModule_Init(&vishnu.timerModule, &vishnu.hardware.interface);
  CombatController_Init(&vishnu.combatController,
                        &vishnu.newSerialMessageEvent,
                        &vishnu.lineDetectedEvent,
                        &vishnu.enemyDetectedEvent,
                        &vishnu.motorRequestEvent,
                        &vishnu.timerModule);
  EnemyDetector_Init(&vishnu.enemyDetector, &vishnu.enemyDetectedEvent, &vishnu.timerModule, &vishnu.hardware.interface);
}

void loop()
{
  SerialReader_ReadLine(&vishnu.serialReader);
  TimerModule_Run(&vishnu.timerModule);
}
