#ifndef __DRIVERS_H__
#define __DRIVERS_H_

#include <stdio.h>
#include "hw_types.h"

/**
 * @brief Initializes the components of the system, such as LEDs, buttons,
 * interruptsm, keyboard and LCD.
 */
void drvComponentInit(void);

int8_t drvLastPressedKey();

bool drvGetReadFlag();

void drvSetReadFlag();

#endif