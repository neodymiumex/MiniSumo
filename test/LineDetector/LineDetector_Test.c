#include "unity.h"
#include "LineDetector.h"
#include "Event.h"
#include "Hardware_Mock.h"

LineDetector_t detector;
Event_t detectedEvent;
Event_t messageEvent;
Timer_t timer;
Hardware_Mock_t hardware;

void setUp(void)
{

}

void tearDown(void)
{

}

void ShouldAddANode(void)
{
    LineDetector_Init(&detector, &detectedEvent, &messageEvent, &timer, &hardware);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(ShouldAddANode);

    return UNITY_END();
}
