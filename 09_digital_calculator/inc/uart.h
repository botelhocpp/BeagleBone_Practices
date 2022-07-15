#ifndef UART_H
#define UART_H

extern void clearTerminal(void);

extern void putCh(char c);

extern char getCh(void);

extern int putString(char *str, unsigned int length);

extern int getString(char *buf, unsigned int length);

#endif