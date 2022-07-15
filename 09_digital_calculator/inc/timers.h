#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>

extern void timerEnable(void);

extern void timerDisable(void);

extern void delay_ms(uint32_t ms);

extern void delay_us(uint32_t us);

extern void timerInitModule(void);

#endif