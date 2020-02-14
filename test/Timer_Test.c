#ifdef UNIT_TEST
#include "unity.h"
#include "Timer.h"

void setUp(void)
{

}

void tearDown(void)
{

}

void ShouldNotCrashWhenStoppingATimerThatIsNotRunning(void)
{
    TimerModule_t timerModule;
    I_Hardware_t hardwareInterface;
    Timer_t timer;

    TimerModule_Init(&timerModule, &hardwareInterface);
    Timer_Stop(&timerModule, &timer);
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(ShouldNotCrashWhenStoppingATimerThatIsNotRunning);

    return UNITY_END();
}
#endif
