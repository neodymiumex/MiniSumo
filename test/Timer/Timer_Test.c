#include "unity.h"
#include "Timer.h"
#include "Hardware_Mock.h"

TimerModule_t timerModule;
Hardware_Mock_t hardware;
Timer_t timer;
Timer_t otherTimer;
bool digitalPinValue;
int analogPinValue;
bool serialAvailable;
char serialChar;
unsigned long currentMs;
Hardware_Mock_Config_t config = {
    .digitalPinValue = &digitalPinValue,
    .analogPinValue = &analogPinValue,
    .serialAvailable = &serialAvailable,
    .serialChar = &serialChar,
    .currentMs = &currentMs
};

int callbackCount;
int otherCallbackCount;

void setUp(void)
{
    callbackCount = 0;
    otherCallbackCount = 0;
    Hardware_Mock_Init(&hardware, &config);
}

void tearDown(void)
{

}

void TheCallback(void *context)
{
    callbackCount++;
}

void TheOtherCallback(void *context)
{
    otherCallbackCount++;
}

void After(unsigned long timeInMs)
{
    Hardware_Mock_ElapseTime(&hardware, timeInMs);
    TimerModule_Run(&timerModule);
}

void ShouldNotCrashWhenStoppingATimerThatIsNotRunning(void)
{
    TimerModule_Init(&timerModule, &hardware.interface);
    Timer_Stop(&timerModule, &timer);
}

void ShouldTriggerCallbackAfterDurationHasExpiredSingle(void)
{
    TimerModule_Init(&timerModule, &hardware.interface);
    Timer_AddSingle(&timerModule, &timer, 10, NULL, TheCallback);

    After(9);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
    After(1);
    TEST_ASSERT_EQUAL_INT(1, callbackCount);
}

void ShouldTriggerCallbackAfterDurationHasExpiredMultipleTimesPeriodic(void)
{
    TimerModule_Init(&timerModule, &hardware.interface);
    Timer_AddPeriodic(&timerModule, &timer, 10, NULL, TheCallback);

    After(9);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
    After(1);
    TEST_ASSERT_EQUAL_INT(1, callbackCount);
    After(9);
    TEST_ASSERT_EQUAL_INT(1, callbackCount);
    After(1);
    TEST_ASSERT_EQUAL_INT(2, callbackCount);
    After(9);
    TEST_ASSERT_EQUAL_INT(2, callbackCount);
    After(1);
    TEST_ASSERT_EQUAL_INT(3, callbackCount);
}

void ShouldNotTriggerACallbackIfTImerIsStoppedSingle(void)
{
    TimerModule_Init(&timerModule, &hardware.interface);
    Timer_AddSingle(&timerModule, &timer, 10, NULL, TheCallback);

    After(9);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
    Timer_Stop(&timerModule, &timer);
    After(1);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
}

void ShouldNotTriggerACallbackIfTimerIsStoppedPeriodic(void)
{
    TimerModule_Init(&timerModule, &hardware.interface);
    Timer_AddPeriodic(&timerModule, &timer, 10, NULL, TheCallback);

    After(9);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
    After(1);
    TEST_ASSERT_EQUAL_INT(1, callbackCount);
    After(9);
    TEST_ASSERT_EQUAL_INT(1, callbackCount);
    Timer_Stop(&timerModule, &timer);
    After(1);
    TEST_ASSERT_EQUAL_INT(1, callbackCount);
    After(10);
    TEST_ASSERT_EQUAL_INT(1, callbackCount);
}

void ShouldHandleMultipleTimers(void)
{
    TimerModule_Init(&timerModule, &hardware.interface);
    Timer_AddSingle(&timerModule, &timer, 7, NULL, TheCallback);
    Timer_AddPeriodic(&timerModule, &otherTimer, 3, NULL, TheOtherCallback);

    After(2);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
    TEST_ASSERT_EQUAL_INT(0, otherCallbackCount);
    After(1);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
    TEST_ASSERT_EQUAL_INT(1, otherCallbackCount);
    After(2);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
    TEST_ASSERT_EQUAL_INT(1, otherCallbackCount);
    After(1);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
    TEST_ASSERT_EQUAL_INT(2, otherCallbackCount);
    After(1);
    TEST_ASSERT_EQUAL_INT(1, callbackCount);
    TEST_ASSERT_EQUAL_INT(2, otherCallbackCount);
    After(2);
    TEST_ASSERT_EQUAL_INT(1, callbackCount);
    TEST_ASSERT_EQUAL_INT(3, otherCallbackCount);
    After(5);
    TEST_ASSERT_EQUAL_INT(1, callbackCount);
    TEST_ASSERT_EQUAL_INT(4, otherCallbackCount);
}

void ShouldHandleAddingTheSameTimerMultipleTimes(void)
{
    TimerModule_Init(&timerModule, &hardware.interface);
    Timer_AddSingle(&timerModule, &timer, 7, NULL, TheCallback);
    Timer_AddSingle(&timerModule, &timer, 3, NULL, TheOtherCallback);

    After(2);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
    TEST_ASSERT_EQUAL_INT(0, otherCallbackCount);
    After(1);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
    TEST_ASSERT_EQUAL_INT(1, otherCallbackCount);
    After(4);
    TEST_ASSERT_EQUAL_INT(0, callbackCount);
    TEST_ASSERT_EQUAL_INT(1, otherCallbackCount);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(ShouldNotCrashWhenStoppingATimerThatIsNotRunning);
    RUN_TEST(ShouldTriggerCallbackAfterDurationHasExpiredSingle);
    RUN_TEST(ShouldTriggerCallbackAfterDurationHasExpiredMultipleTimesPeriodic);
    RUN_TEST(ShouldNotTriggerACallbackIfTImerIsStoppedSingle);
    RUN_TEST(ShouldNotTriggerACallbackIfTimerIsStoppedPeriodic);
    RUN_TEST(ShouldHandleMultipleTimers);
    RUN_TEST(ShouldHandleAddingTheSameTimerMultipleTimes);

    return UNITY_END();
}
