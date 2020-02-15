#include "unity.h"
#include "MotorController.h"
#include "Hardware_Mock.h"
#include "Event.h"

MotorController_t controller;
Event_t requestEvent;
Event_t messageEvent;
Hardware_Mock_t hardware;
int pinMode[6];
bool digitalPinValue[6];
int analogPinValue[6];
bool serialAvailable;
char serialChar[20];
unsigned long currentMs;
int eventCount;
char *messageWritten;
Hardware_Mock_Config_t config;

void setUp(void)
{
    eventCount = 0;
    config.digitalPinValue = &digitalPinValue;
    config.analogPinValue = &analogPinValue;
    config.serialAvailable = &serialAvailable;
    config.serialChar = serialChar;
    config.currentMs = &currentMs;

    for(int i=0; i < 6; i++)
    {
        pinMode[i] = 0;
        digitalPinValue[i] = 0;
        analogPinValue[i] = 0;
    }

    Hardware_Mock_Init(&hardware, &config);
    Event_Init(&requestEvent);
    Event_Init(&messageEvent);
}

void tearDown(void)
{

}

void ShouldHandleMotorRequest(void)
{
    MotorRequest_t request;
    MotorController_Init(&controller, &requestEvent, &messageEvent, &hardware.interface);
    request.left = MS_GoSlow;
    request.right = MS_GoFast;
    Event_Publish(&requestEvent, (const void *)&request);
    TEST_ASSERT_EQUAL_INT(MS_GoSlow, analogPinValue[MOTOR_1_EN]);
    TEST_ASSERT_EQUAL_INT(MS_GoFast, analogPinValue[MOTOR_2_EN]);
}

void ShouldHandleMessage(void)
{
    char *message = "Set Motors X 20 Y 100";
    MotorController_Init(&controller, &requestEvent, &messageEvent, &hardware.interface);
    Event_Publish(&messageEvent, message);
    TEST_ASSERT_EQUAL_INT(83, analogPinValue[MOTOR_1_EN]);
    TEST_ASSERT_EQUAL_INT(99, analogPinValue[MOTOR_2_EN]);
}

void ShouldNotReactToBadMessage(void)
{
    char *message = "Set Robot X 20 Y 100";
    MotorController_Init(&controller, &requestEvent, &messageEvent, &hardware.interface);
    Event_Publish(&messageEvent, message);
    TEST_ASSERT_EQUAL_INT(0, analogPinValue[MOTOR_1_EN]);
    TEST_ASSERT_EQUAL_INT(0, analogPinValue[MOTOR_2_EN]);
}

void ShouldHandleMessageWithDifferentParams(void)
{
    char *message = "Set Motors X 20 Y 20";
    MotorController_Init(&controller, &requestEvent, &messageEvent, &hardware.interface);
    Event_Publish(&messageEvent, message);
    TEST_ASSERT_EQUAL_INT(2, analogPinValue[MOTOR_1_EN]);
    TEST_ASSERT_EQUAL_INT(19, analogPinValue[MOTOR_2_EN]);
}

void ShouldHandleMessageWithNegativeParams(void)
{
    char *message = "Set Motors X -20 Y 20";
    MotorController_Init(&controller, &requestEvent, &messageEvent, &hardware.interface);
    Event_Publish(&messageEvent, message);
    TEST_ASSERT_EQUAL_INT(19, analogPinValue[MOTOR_1_EN]);
    TEST_ASSERT_EQUAL_INT(2, analogPinValue[MOTOR_2_EN]);
}

void SHouldHandleMessageForReverse(void)
{
    char *message = "Set Motors X 0 Y -20";
    MotorController_Init(&controller, &requestEvent, &messageEvent, &hardware.interface);
    Event_Publish(&messageEvent, message);
    TEST_ASSERT_EQUAL_INT(-12, analogPinValue[MOTOR_1_EN]);
    TEST_ASSERT_EQUAL_INT(-12, analogPinValue[MOTOR_2_EN]);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(ShouldHandleMotorRequest);
    RUN_TEST(ShouldHandleMessage);
    RUN_TEST(ShouldNotReactToBadMessage);
    RUN_TEST(ShouldHandleMessageWithDifferentParams);
    RUN_TEST(ShouldHandleMessageWithNegativeParams);
    RUN_TEST(SHouldHandleMessageForReverse);

    return UNITY_END();
}
