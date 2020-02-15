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
Hardware_Mock_Config_t config = {
    .digitalPinValue = &digitalPinValue,
    .analogPinValue = &analogPinValue,
    .serialAvailable = &serialAvailable,
    .serialChar = &serialChar,
    .currentMs = &currentMs
};
int eventCount;
EventSubscription_t subscription;
char *messageRead;

void TheCallback(void *context, void *args)
{
    messageRead = (char *)args;
    eventCount++;
}

void setUp(void)
{
    eventCount = 0;
    Hardware_Mock_Init(&hardware, &config);
    EventSubscription_Init(&subscription, NULL, TheCallback);
    Event_Init(&publishEvent);
    Event_Subscribe(&publishEvent, &subscription);
}

void tearDown(void)
{

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

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(ShouldPublishAnEventWithReadData);

    return UNITY_END();
}
