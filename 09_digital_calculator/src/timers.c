#include "timers.h"

#include "clock_module.h"
#include "control_module.h"
#include "interrupt.h"

#include <stdint.h>

#define TIMER_1US_COUNT                 		24
#define TIMER_1MS_COUNT                 		24000
#define TIMER_OVERFLOW                  		0xFFFFFFFFu

#define DMTimerWaitForWrite(reg)   \
            if(HWREG(DMTIMER_TSICR) & (1U << 2))\
            while((reg & HWREG(DMTIMER_TWPS)));

/* DMTIMER */
#define DMTIMER_TIDR   						0x4804A000
#define DMTIMER_TIOCP_CFG   					0x4804A010
#define DMTIMER_IRQ_EOI   					0x4804A020
#define DMTIMER_IRQSTATUS_RAW   				0x4804A024
#define DMTIMER_IRQSTATUS   					0x4804A028
#define DMTIMER_IRQENABLE_SET   				0x4804A02C
#define DMTIMER_IRQENABLE_CLR   				0x4804A030
#define DMTIMER_IRQWAKEEN  	 				0x4804A034
#define DMTIMER_TCLR   						0x4804A038
#define DMTIMER_TCRR   						0x4804A03C
#define DMTIMER_TLDR   						0x4804A040
#define DMTIMER_TTGR   						0x4804A044
#define DMTIMER_TWPS   						0x4804A048
#define DMTIMER_TMAR   						0x4804A04C
#define DMTIMER_TCAR(n)   					(0x50 + (((n) - 1) * 8))
#define DMTIMER_TSICR  		 				0x4804A054
#define CM_PER_TIMER7_CLKCTRL                     0x44E0007C
#define SOC_CM_DPLL_CLKSEL_TIMER7_CLK             0x44E00504

#include "hw_types.h"

void timerEnable(void){
    /* Wait for previous write to complete in TCLR */
	DMTimerWaitForWrite(0x1);

    /* Start the timer */
    HWREG(DMTIMER_TCLR) |= 0x1;
}

void timerDisable(void){
    /* Wait for previous write to complete in TCLR */
	DMTimerWaitForWrite(0x1);

    /* Start the timer */
    HWREG(DMTIMER_TCLR) &= ~(0x1);
}

void delay_ms(uint32_t ms){
    while(ms != 0){
        /* Wait for previous write to complete */
        DMTimerWaitForWrite(0x2);

        /* Set the counter value. */
        HWREG(DMTIMER_TCRR) = 0x0;

        timerEnable();

        while(HWREG(DMTIMER_TCRR) < TIMER_1MS_COUNT);

        /* Stop the timer */
        HWREG(DMTIMER_TCLR) &= ~(0x00000001u);

        ms--;
    }
}

void delay_us(uint32_t us){
    while(us != 0){
        /* Wait for previous write to complete */
        DMTimerWaitForWrite(0x2);

        /* Set the counter value. */
        HWREG(DMTIMER_TCRR) = 0x0;

        timerEnable();

        while(HWREG(DMTIMER_TCRR) < TIMER_1US_COUNT);

        /* Stop the timer */
        HWREG(DMTIMER_TCLR) &= ~(0x00000001u);

        us--;
    }
}

void timerInitModule(void){
     ckmSetCLKModuleRegister(CKM_PER, CKM_PER_TIMER7_CLKCTRL, (0b10U << 0));
     while ((ckmGetCLKModuleRegister(CKM_PER, CKM_PER_TIMER7_CLKCTRL) & (0b11U << 0)) != 2);
}