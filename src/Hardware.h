#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include "I_Hardware.h"

typedef struct
{
    I_Hardware_t interface;
} Hardware_t;

void Hardware_Init(Hardware_t *instance);

#endif
