/**
 * @file system.h
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @brief Configuration functions of the system.
 * @version 1.0
 * @date 2022-06-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

extern void systemInit(void);

extern void delay_ms(uint32_t ms);

extern void irqHandler(void);

extern void irqHandlerGpio1A(void);

extern void irqHandlerGpio1B(void);

extern void irqHandlerGpio2A(void);

extern void irqHandlerGpio2B(void);

extern void irqHandlerDMTimer7(void);

#endif