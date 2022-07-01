/**
 * @file uart.h
 * @author Francisco Helder (helderhdw@gmail.com)
 * @brief Serial interface access functions.
 * @version 1.0
 * @date 2022-06-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef UART_H
#define UART_H

#include "uart.h"

extern void putCh(char c);

extern char getCh(void);

extern int putString(char *str, unsigned int length);

extern int getString(char *buf, unsigned int length);

#endif