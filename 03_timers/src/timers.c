#include "timers.h"

#include <stdint.h>

#include "bbb_regs.h"
#include "hw_types.h"

// =============================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// =============================================================================

void timerStartCount(void);

// =============================================================================
// PUBLIC FUNCTIONS IMPLEMENTATIONS
// =============================================================================

void timerSetup(void) {
     /*  Enable clock for DMTIMER7. */
     HWREG(CM_PER_TIMER7_CLKCTRL) |= (0b10U << 0);

	/*  Wait for clock to be enabled to DMTIMER7. */    
     while((HWREG(CM_PER_TIMER7_CLKCTRL) & 0b11U) != (0b10U << 0));

     /* Enable interupts for DMTIMER7 (95). */
     HWREG(INTC_MIR_CLEAR2) |= (1U << 31);

     /* Enable DMTimer Interrupts on Overflow. */
     HWREG(DMTIMER_IRQENABLE_SET) |= (1U << 1);

     /* Enable auto-reload. */
     HWREG(DMTIMER_TCLR) |= (1U << 1);
}

void timerStart(uint32_t ms) {
     /* Reload value of counter. */
     uint32_t reload = TIMER_OVERFLOW - (ms*TIMER_1MS_COUNT);

     /* If posted mode is active wait for pending write to TCRR to complete. */
     if(HWREG(DMTIMER_TSICR) & 0b1000U) {
          while(HWREG(DMTIMER_TWPS) & 0b10U);
     }

     /* Load the Counter and Reload registers with the counter value. */
     HWREG(DMTIMER_TCRR) = reload;
     HWREG(DMTIMER_TLDR) = reload;

     timerStartCount();
}

void timerStop(void) {
     /* If posted mode is active wait for pending write to TCLR to complete. */
     if(HWREG(DMTIMER_TSICR) & 0b1000U) {
          while(HWREG(DMTIMER_TWPS) & 0b1U);
     }
     /* Stops the timer */
     HWREG(DMTIMER_TCLR) &= ~(1U << 0);
}

// =============================================================================
// PRIVATE FUNCTIONS IMPLEMENTATIONS
// =============================================================================

void timerStartCount(void) {
     /* If posted mode is active wait for pending write to TCLR to complete. */
     if(HWREG(DMTIMER_TSICR) & 0b1000U) {
          while(HWREG(DMTIMER_TWPS) & 0b1U);
     }
     /* Start the timer */
     HWREG(DMTIMER_TCLR) |= (1U << 0);
}