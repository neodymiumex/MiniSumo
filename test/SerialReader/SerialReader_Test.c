#include "unity.h"
#include "SerialReader.h"
#include "Hardware_Mock.h"
#include "Event.h"

SerialReader_t reader;
Event_t publishEvent;
Hardware_Mock_t hardware;
bool digitalPinValue;
int analogPinValue;
bool serialAvailable;
char serialChar[20];
unsigned long currentMs;
int eventCount;
EventSubscription_t subscription;
char *messageRead;
Hardware_Mock_Config_t config;

void TheCallback(void *context, void *args)
{
    messageRead = (char *)args;
    eventCount++;
}

void setUp(void)
{
    eventCount = 0;
    config.digitalPinValue = &digitalPinValue;
    config.analogPinValue = &analogPinValue;
    config.serialAvailable = &serialAvailable;
    config.serialChar = serialChar;
    config.currentMs = &currentMs;
    Hardware_Mock_Init(&hardware, &config);
    EventSubscription_Init(&subscription, NULL, TheCallback);
    Event_Init(&publishEvent);
    Event_Subscribe(&publishEvent, &subscription);
}

void tearDown(void)
{
    Event_Unsubscribe(&publishEvent, &subscription);
}

void ShouldPublishAnEventWithReadData(void)
{
    char *message = "This is the end\n";
    char *expected = "This is the end";
    SerialReader_Init(&reader, &publishEvent, &hardware.interface);
    Hardware_WriteLineToSerial1(&hardware.interface, message);
    TEST_ASSERT_EQUAL_INT(0, eventCount);
    SerialReader_ReadLine(&reader);
    TEST_ASSERT_EQUAL_INT(1, eventCount);
    TEST_ASSERT_EQUAL_STRING(expected, messageRead);
}

void ShouldHandleMessageInTwoParts(void)
{
    char *expected = "One in a million";
    char *partOne = "One i";
    char *partTwo = "n a million\n";
    SerialReader_Init(&reader, &publishEvent, &hardware.interface);
    Hardware_WriteLineToSerial1(&hardware.interface, partOne);
    TEST_ASSERT_EQUAL_INT(0, eventCount);
    SerialReader_ReadLine(&reader);
    TEST_ASSERT_EQUAL_INT(0, eventCount);
    Hardware_WriteLineToSerial1(&hardware.interface, partTwo);
    SerialReader_ReadLine(&reader);
    TEST_ASSERT_EQUAL_INT(1, eventCount);
    TEST_ASSERT_EQUAL_STRING(expected, messageRead);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(ShouldPublishAnEventWithReadData);
    RUN_TEST(ShouldHandleMessageInTwoParts);

    return UNITY_END();
}
