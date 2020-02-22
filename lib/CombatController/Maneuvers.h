#ifndef _MANUEVERS_H_
#define _MANUEVERS_H_

#include <stdint.h>
#include "MotorController.h"

#define MS_PER_SECOND 1000

typedef enum
{
    MT_Go,
    MT_Attack,
    MT_Brake,
    MT_Reverse,
    MT_TurnAroundLeft,
    MT_TurnAroundRight,
    MT_EscapeLeft,
    MT_EscapeRight,
    MT_BreakDeadlock,
    MT_Max
} ManeuverType_t;

typedef enum
{
    Step_Forward,
    Step_ForwaredMax,
    Step_ForwardLeft,
    Step_ForwardRight,
    Step_HardLeft,
    Step_HardRight,
    Step_SpinLeft,
    Step_SpinRight,
    Step_Stop,
    Step_Brake,
    Step_Reverse,
    Step_ReverseMax,
    Step_ReverseLeft,
    Step_ReverseRight,
    Step_Max
} ManeuverStep_t;

typedef struct
{
    ManeuverStep_t name;
    MotorSpeed_t left;
    MotorSpeed_t right;
} Step_t;

typedef struct
{
    ManeuverStep_t step;
    unsigned long durationMs;
} TimedStep_t;

typedef struct
{
    const TimedStep_t *steps;
    const uint8_t stepCount;
} Maneuver_t;

const Step_t StepsList[Step_Max] = {
    { Step_Forward,      MS_GoMedium,       MS_GoMedium },
    { Step_ForwaredMax,  MS_GoFast,         MS_GoFast },
    { Step_ForwardLeft,  MS_GoMedium,       MS_GoFast },
    { Step_ForwardRight, MS_GoFast,         MS_GoMedium },
    { Step_HardLeft,     MS_GoSlow,         MS_GoFast },
    { Step_HardRight,    MS_GoFast,         MS_GoSlow },
    { Step_SpinLeft,     MS_ReverseMedium,  MS_GoMedium },
    { Step_SpinRight,    MS_GoMedium,       MS_ReverseMedium },
    { Step_Stop,         MS_Stop,           MS_Stop },
    { Step_Brake,        MS_ReverseSlow,    MS_ReverseSlow },
    { Step_Reverse,      MS_ReverseMedium,  MS_ReverseMedium },
    { Step_ReverseMax,   MS_ReverseFast,    MS_ReverseFast },
    { Step_ReverseLeft,  MS_ReverseMedium,  MS_ReverseFast },
    { Step_ReverseRight, MS_ReverseFast,    MS_ReverseMedium }
};

const TimedStep_t forwardSteps[] = {
    { Step_Forward, 6 * MS_PER_SECOND }
};

const Maneuver_t M_Forward = {
    forwardSteps,
    sizeof(forwardSteps)
};

const TimedStep_t attackSteps[] = {
    { Step_ForwaredMax, 15 * MS_PER_SECOND }
};

const Maneuver_t M_Attack = {
    attackSteps,
    sizeof(attackSteps)
};

const TimedStep_t brakeSteps[] = {
    { Step_Brake, 5 }
};

const Maneuver_t M_Brake = {
    brakeSteps,
    sizeof(brakeSteps)
};

const TimedStep_t reverseSteps[] = {
    { Step_Brake, 5 },
    { Step_Reverse, 1 * MS_PER_SECOND }
};

const Maneuver_t M_Reverse = {
    reverseSteps,
    sizeof(reverseSteps)
};

const TimedStep_t turnaroundLeftSteps[] = {
    { Step_Brake, 5 },
    { Step_Reverse, 30 },
    { Step_ReverseLeft, 50 }
};

const Maneuver_t M_TurnaroundLeft = {
    turnaroundLeftSteps,
    sizeof(turnaroundLeftSteps)
};

const TimedStep_t turnaroundRightSteps[] = {
    { Step_Brake, 5 },
    { Step_Reverse, 30 },
    { Step_ReverseRight, 50 }
};

const Maneuver_t M_TurnaroundRight = {
    turnaroundRightSteps,
    sizeof(turnaroundRightSteps)
};

const TimedStep_t escapeLeftSteps[] = {
    { Step_Brake, 5 },
    { Step_ReverseMax, 30 },
    { Step_ReverseLeft, 20 }
};

const Maneuver_t M_EscapeLeft = {
    escapeLeftSteps,
    sizeof(escapeLeftSteps)
};

const TimedStep_t escapeRightSteps[] = {
    { Step_Brake, 5 },
    { Step_ReverseMax, 30 },
    { Step_ReverseRight, 20 }
};

const Maneuver_t M_EscapeRight = {
    escapeRightSteps,
    sizeof(escapeRightSteps)
};

const TimedStep_t breakDeadlockSteps[] = {
    { Step_ReverseMax, 10 }
};

const Maneuver_t M_BreakDeadlock = {
    breakDeadlockSteps,
    sizeof(breakDeadlockSteps)
};

const Maneuver_t *ManeuverList[MT_Max] = {
    &M_Forward,
    &M_Attack,
    &M_Brake,
    &M_Reverse,
    &M_TurnaroundLeft,
    &M_TurnaroundRight,
    &M_EscapeLeft,
    &M_EscapeRight,
    &M_BreakDeadlock
};

#endif
