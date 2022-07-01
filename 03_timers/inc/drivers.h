#ifndef DRIVERS_H
#define DRIVERS_H

#include "bbb_regs.h"
#include "hw_types.h"

#include <stdint.h>

#define NUMBER_CURSOR_POSITIONS 4

#define BUTTON_01_BIT                   12
#define BUTTON_01_MASK                  (1U << BUTTON_01_BIT)
#define BUTTON_01_DATAIN                GPIO1_DATAIN

#define BUTTON_02_BIT                   14
#define BUTTON_02_MASK                  (1U << BUTTON_02_BIT)
#define BUTTON_02_DATAIN                GPIO1_DATAIN

#define LED_01_BIT					24
#define LED_01_MASK					(1U << LED_01_BIT)
#define LED_01_DATAOUT				GPIO1_DATAOUT

#define LED_02_BIT					23
#define LED_02_MASK					(1U << LED_02_BIT)
#define LED_02_DATAOUT				GPIO1_DATAOUT

#define LED_03_BIT					22
#define LED_03_MASK					(1U << LED_03_BIT)
#define LED_03_DATAOUT				GPIO1_DATAOUT

#define LED_04_BIT					21
#define LED_04_MASK					(1U << LED_04_BIT)
#define LED_04_DATAOUT				GPIO1_DATAOUT

#define LED_05_BIT					29
#define LED_05_MASK					(1U << LED_05_BIT)
#define LED_05_DATAOUT				GPIO1_DATAOUT

#define LED_06_BIT					13
#define LED_06_MASK					(1U << LED_06_BIT)
#define LED_06_DATAOUT				GPIO1_DATAOUT

#define LED_07_BIT					16
#define LED_07_MASK					(1U << LED_07_BIT)
#define LED_07_DATAOUT				GPIO1_DATAOUT

#define LED_08_BIT					17
#define LED_08_MASK					(1U << LED_08_BIT)
#define LED_08_DATAOUT				GPIO1_DATAOUT

typedef enum {
	BUTTON_01,
	BUTTON_02
} button_id;

typedef enum {
	LED_01,
	LED_02,
	LED_03,
	LED_04,
	LED_05,
	LED_06,
	LED_07,
	LED_08
} led_id;

typedef enum {
	LED_OFF,
	LED_ON
} led_level;

extern void buttonConfig(void);

extern void ledConfig(void);

extern bool readButton(button_id btn);

extern void ledOff(led_id led);

extern void ledOn(led_id led);

extern void toggleLeds(void);

extern uint32_t getBlinkTime(void);

extern void scrollCursorDown(void);

extern void selectCurrentOption(void);

extern uint8_t getCursorPosition();

#endif