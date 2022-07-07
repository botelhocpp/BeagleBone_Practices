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

/*-----------------------------------------------------------------------------
 *  init ENUMs
 *-----------------------------------------------------------------------------*/
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
void gpioPInitPin(gpio_handle_t *pin, pin_direction direction);
void gpioInitModule(gpio_port port);
void gpioInitPin(gpio_handle_t *pin);
void gpioSetPinDirection(gpio_handle_t *pin, pin_direction direction);	
pin_direction gpioGetPinDirection(gpio_handle_t *pin);
void gpioSetPinValue(gpio_handle_t *pin, pin_level level);
pin_level gpioGetPinValue(gpio_handle_t *pin);

#endif /*GPIO_H_ */
