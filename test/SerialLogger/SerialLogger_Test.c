#include "unity.h"
#include "SerialLogger.h"
#include "Hardware_Mock.h"
#include "Event.h"

SerialLogger_t logger;
Event_t logEvent;
Hardware_Mock_t hardware;
bool digitalPinValue;
int analogPinValue;
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
    Hardware_Mock_Init(&hardware, &config);
    Event_Init(&logEvent);
}

void tearDown(void)
{

}

void ShouldWriteMessageOnEvent(void)
{
    char *message = "Don't screw it up\n";
    SerialLogger_Init(&logger, &logEvent, &hardware);
    Event_Publish(&logEvent, message);
    TEST_ASSERT_EQUAL_STRING(message, serialChar);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(ShouldWriteMessageOnEvent);

    return UNITY_END();
}
