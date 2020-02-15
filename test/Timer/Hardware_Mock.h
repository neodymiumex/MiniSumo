#ifndef _HARDWARE_MOCK_H_
#define _HARDWARE_MOCK_H_

#include "I_Hardware.h"

typedef struct
{
    bool *digitalPinValue;
    int *analogPinValue;
    bool *serialAvailable;
    char *serialChar;
    unsigned long *currentMs;
} Hardware_Mock_Config_t;

typedef struct
{
    I_Hardware_t interface;
    Hardware_Mock_Config_t *config;
} Hardware_Mock_t;

void Hardware_Mock_Init(Hardware_Mock_t *instance, Hardware_Mock_Config_t  *config);
void Hardware_Mock_ElapseTime(Hardware_Mock_t *instance, unsigned long timeElapsed);

#endif
