#include "gpio.h"

#include "hw_types.h"
#include "bbb_regs.h"

// =============================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// =============================================================================

void gpio1Setup(void);

void gpio2Setup(void);

// =============================================================================
// PUBLIC FUNCTIONS IMPLEMENTATIONS
// =============================================================================

void gpioSetup(){
     gpio1Setup();
     gpio2Setup();
}

// =============================================================================
// PRIVATE FUNCTIONS IMPLEMENTATIONS
// =============================================================================

void gpio1Setup(void) {
	/* Enable Clock for GPIO1 */
	HWREG(CM_PER_GPIO1_CLKCTRL) = (1U << 18) | (0b10U << 0);

     /* Unmask Interrupts for GPIO1A (98) and GPIO1B (99) */
    	HWREG(INTC_MIR_CLEAR3) = (1U << 3) | (1U << 2);

	/* Set interrupts to IRQ and give them top priority. */
	HWREG(INTC_ILR_98) &= ~(1U << 0) & ~(0x3FU << 2);
	HWREG(INTC_ILR_99) &= ~(1U << 0) & ~(0x3FU << 2);
}

void gpio2Setup(void) {
	/* Enable Clock for GPIO2 */
	HWREG(CM_PER_GPIO2_CLKCTRL) = (1U << 18) | (0b10U << 0);

     /* Unmask Interrupts for GPIO2A (32) and GPIO2B (33) */
    	HWREG(INTC_MIR_CLEAR1) = (1U << 1) | (1U << 0);

	/* Set interrupts to IRQ and give them top priority. */
	HWREG(INTC_ILR_32) &= ~(1U << 0) & ~(0x3FU << 2);
	HWREG(INTC_ILR_33) &= ~(1U << 0) & ~(0x3FU << 2);
}