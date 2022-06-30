#ifndef DRIVERS_H
#define DRIVERS_H

#include "bbb_regs.h"
#include "hw_types.h"

#define BUTTON_01_BIT                   12
#define BUTTON_01_MASK                  (1U << BUTTON_01_BIT)
#define BUTTON_01_DATAIN                GPIO1_DATAIN

#define BUTTON_02_BIT                   14
#define BUTTON_02_MASK                  (1U << BUTTON_02_BIT)
#define BUTTON_02_DATAIN                GPIO1_DATAIN

#define BUTTON_03_BIT                   1
#define BUTTON_03_MASK                  (1U << BUTTON_03_BIT)
#define BUTTON_03_DATAIN                GPIO2_DATAIN

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

typedef enum {
	BUTTON_01,
	BUTTON_02,
	BUTTON_03
} button_id;

typedef enum {
	LED_01,
	LED_02,
	LED_03,
	LED_04
} led_id;

extern void buttonConfig(void);

extern void ledConfig(void);

extern bool readButton(button_id btn);

extern void ledOff(led_id led);

extern void ledOn(led_id led);

extern void ledSequence01(void);

extern void ledSequence02(void);

extern void ledSequence03(void);

extern void ledSequence04(void);

extern void buttonLedTest(void);

extern int getBlinkMode();

extern void setBlinkMode(int mode);

#endif