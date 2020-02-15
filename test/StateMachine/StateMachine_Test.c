#include "unity.h"
#include "StateMachine.h"

enum {
    SM_Signal = SM_SIGNAL_START
};

StateMachine_t stateMachine;
int stateSignalCount;
int otherStateSignalCount;
int stateArgs;
int entryCount;
int exitCount;
int otherEntryCount;
int otherExitCount;

void setUp(void)
{
    stateSignalCount = 0;
    otherStateSignalCount = 0;
    stateArgs = 0;
    entryCount = 0;
    exitCount = 0;
    otherEntryCount = 0;
    otherExitCount = 0;
}

void tearDown(void)
{

}

static void TheState(void *context, Signal_t signal, const void *args);
static void TheOtherState(void *context, Signal_t signal, const void *args);

static void TheState(void *context, Signal_t signal, const void *args)
{
    switch(signal)
    {
        case SM_ENTRY:
            entryCount++;
            break;

        case SM_Signal:
            if(args)
            {
                stateArgs = *(int *)args;
            }
            stateSignalCount++;
            break;

        case SM_EXIT:
            exitCount++;
            break;
    }
}

static void TheOtherState(void *context, Signal_t signal, const void *args)
{
    switch(signal)
    {
        case SM_ENTRY:
            otherEntryCount++;
            break;

        case SM_Signal:
            otherStateSignalCount++;
            break;

        case SM_EXIT:
            otherExitCount++;
            break;
    }
}

void ShouldSendSignalToInitialState(void)
{
    StateMachine_Init(&stateMachine, TheState);
    TEST_ASSERT_EQUAL_INT(0, stateSignalCount);
    StateMachine_Signal(&stateMachine, SM_Signal, NULL);
    TEST_ASSERT_EQUAL_INT(1, stateSignalCount);
}

void ShouldSendSignalToTransitionedState(void)
{
    StateMachine_Init(&stateMachine, TheState);
    StateMachine_Transition(&stateMachine, TheOtherState);
    TEST_ASSERT_EQUAL_INT(0, otherStateSignalCount);
    StateMachine_Signal(&stateMachine, SM_Signal, NULL);
    TEST_ASSERT_EQUAL_INT(1, otherStateSignalCount);
}

void ShouldPassArgsWithSignal(void)
{
    int signalArg = 5;
    StateMachine_Init(&stateMachine, TheState);
    TEST_ASSERT_EQUAL_INT(0, stateArgs);
    StateMachine_Signal(&stateMachine, SM_Signal, &signalArg);
    TEST_ASSERT_EQUAL_INT(5, stateArgs);
}

void ShouldCallEntryAndExitOnTransition(void)
{
    StateMachine_Init(&stateMachine, TheState);
    TEST_ASSERT_EQUAL_INT(0, entryCount);
    TEST_ASSERT_EQUAL_INT(0, exitCount);
    TEST_ASSERT_EQUAL_INT(0, otherEntryCount);
    TEST_ASSERT_EQUAL_INT(0, otherExitCount);
    StateMachine_Transition(&stateMachine, TheOtherState);
    TEST_ASSERT_EQUAL_INT(0, entryCount);
    TEST_ASSERT_EQUAL_INT(1, exitCount);
    TEST_ASSERT_EQUAL_INT(1, otherEntryCount);
    TEST_ASSERT_EQUAL_INT(0, otherExitCount);
    StateMachine_Transition(&stateMachine, TheState);
    TEST_ASSERT_EQUAL_INT(1, entryCount);
    TEST_ASSERT_EQUAL_INT(1, exitCount);
    TEST_ASSERT_EQUAL_INT(1, otherEntryCount);
    TEST_ASSERT_EQUAL_INT(1, otherExitCount);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(ShouldSendSignalToInitialState);
    RUN_TEST(ShouldSendSignalToTransitionedState);
    RUN_TEST(ShouldPassArgsWithSignal);
    RUN_TEST(ShouldCallEntryAndExitOnTransition);

    return UNITY_END();
}
