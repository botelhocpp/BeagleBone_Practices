#ifndef SYSTEM_H
#define SYSTEM_H

extern void systemInit(void);

extern void delay_ms(unsigned int ms);

extern void irqHandler(void);

extern void irqHandlerGpio1A(void);

extern void irqHandlerGpio1B(void);

extern void irqHandlerGpio2A(void);

extern void irqHandlerGpio2B(void);

#endif