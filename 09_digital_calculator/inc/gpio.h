/**
 * @file gpio.h
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @author Francisco Helder (helderhdw@gmail.com)
 * @brief Provide functions for controlling the GPIO
 * peripheral through a simple structure.
 * @version 2.0
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef GPIO_H_
#define GPIO_H_

#include "clock_module.h"
#include "control_module.h"
#include "pad.h"
#include "soc_AM335x.h"
#include "hw_types.h"

#include <stdint.h>

#define GPIO_IRQSTATUS_SET(n)   (0x34 + (n * 4))
#define GPIO_IRQSTATUS_CLR(n)   (0x3C + (n * 4))
#define GPIO_LEVELDETECT(n)     (0x140 + (n * 4))

#define GPIO_REVISION           0x000
#define GPIO_SYSCONFIG          0x010
#define GPIO_EOI                0x020
#define GPIO_IRQSTATUS_RAW_0    0x024
#define GPIO_IRQSTATUS_RAW_1    0x028
#define GPIO_IRQSTATUS_0        0x02C
#define GPIO_IRQSTATUS_1        0x030
#define GPIO_IRQSTATUS_SET_0    0x034
#define GPIO_IRQSTATUS_SET_1    0x038
#define GPIO_IRQSTATUS_CLR_0    0x03C
#define GPIO_IRQSTATUS_CLR_1    0x040
#define GPIO_IRQWAKEN_0         0x044
#define GPIO_IRQWAKEN_1         0x048
#define GPIO_SYSSTATUS          0x114
#define GPIO_CTRL               0x130
#define GPIO_OE                 0x134
#define GPIO_DATAIN             0x138
#define GPIO_DATAOUT            0x13C
#define GPIO_LEVELDETECT0       0x140
#define GPIO_LEVELDETECT1       0x144
#define GPIO_RISINGDETECT       0x148
#define GPIO_FALLINGDETECT      0x14C
#define GPIO_DEBOUNCENABLE      0x150
#define GPIO_DEBOUNCINGTIME     0x154
#define GPIO_CLEARDATAOUT       0x190
#define GPIO_SETDATAOUT         0x194

/* Values denoting the Interrupt Line number to be used. */
#define GPIO_INTC_LINE_1                  (0x0)
#define GPIO_INTC_LINE_2                  (0x1)

/*
** Values used to enable/disable interrupt generation due to level
** detection on an input GPIO pin.
*/
#define GPIO_INTC_TYPE_NO_LEVEL           (0x01)
#define GPIO_INTC_TYPE_LEVEL_LOW          (0x04)
#define GPIO_INTC_TYPE_LEVEL_HIGH         (0x08)
#define GPIO_INTC_TYPE_BOTH_LEVEL         (0x0C)

/*
** Values used to enable/disable interrupt generation due to edge
** detection on an input GPIO pin.
*/
#define GPIO_INTC_TYPE_NO_EDGE            (0x80)
#define GPIO_INTC_TYPE_RISE_EDGE          (0x10)
#define GPIO_INTC_TYPE_FALL_EDGE          (0x20)
#define GPIO_INTC_TYPE_BOTH_EDGE          (0x30)

/*-----------------------------------------------------------------------------
 *  init ENUMs
 *-----------------------------------------------------------------------------*/
typedef enum {
	PULLDOWN,
	PULLUP
} pin_pull;

typedef enum {
	LOW,
	HIGH
} pin_level;

typedef enum {
	GPIO0,
	GPIO1,
	GPIO2,
	GPIO3
} gpio_port;

typedef enum {
	OUTPUT,
	INPUT
} pin_direction;

typedef struct {
	gpio_port port;
	uint8_t pin_number;
} gpio_handle_t;

/*-----------------------------------------------------------------------------
 *  Function prototype
 *-----------------------------------------------------------------------------*/

void gpioFInitPin(gpio_handle_t *pin, pin_direction direction);

/**
 * @brief Initialize the given pin with the given direction.
 * 
 * @param pin 
 * @param direction 
 */
void gpioPInitPin(gpio_handle_t *pin, pin_direction direction);
void gpioInitModule(gpio_port port);
void gpioInitPin(gpio_handle_t *pin);
void gpioConfigPull(gpio_handle_t *pin, pin_pull pull_type);

void gpioSetPinDirection(gpio_handle_t *pin, pin_direction direction);	
pin_direction gpioGetPinDirection(gpio_handle_t *pin);
void gpioTogglePinValue(gpio_handle_t *pin);
void gpioSetPinValue(gpio_handle_t *pin, pin_level level);
pin_level gpioGetPinValue(gpio_handle_t *pin);

void gpioAintcConfigure(uint32_t int_line, uint32_t priority, void (*fnHandler)(void));
void gpioPinIntEnable(gpio_handle_t *pin, uint32_t int_line);
void gpioIntTypeSet(gpio_handle_t *pin, uint32_t event_type);
bool gpioCheckIntFlag(gpio_handle_t *pin, uint32_t int_line);
void gpioClearIntFlag(gpio_handle_t *pin, uint32_t int_line);

#endif /*GPIO_H_ */
