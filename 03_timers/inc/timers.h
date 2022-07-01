#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>

extern void timerSetup(void);

extern void timerStart(uint32_t ms);

extern void timerStop(void);

#endif