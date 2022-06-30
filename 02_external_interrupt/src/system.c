#include "system.h"
#include "drivers.h"
#include "uart.h"

#include "bbb_regs.h"
#include "hw_types.h"

// =============================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// =============================================================================

void intConfig(void);

void disableWdt(void);

// =============================================================================
// PUBLIC FUNCTIONS IMPLEMENTATIONS
// =============================================================================

void systemInit(void) {
     disableWdt();
     // intConfig();
}

void delay_ms(unsigned int ms) {
	for(unsigned int i = 0; i < ms; i++) {
		for(unsigned int j = 0; j < 300000; j++);
	}
}

void irqHandler(void){

	putString("IRQ HANDLER!\r\n", 15);

	/* Verify active IRQ number */
	unsigned int irq_number = HWREG(INTC_SIR_IRQ) & 0x7F;
	
	if(irq_number == 32) {
		irqHandlerGpio2A();
	}
	else if(irq_number == 33) {
		irqHandlerGpio2B();
	}
	else if(irq_number == 98) {
		irqHandlerGpio1A();
	}
	else if(irq_number == 99) {
		irqHandlerGpio1B();
	}

	/* acknowledge IRQ */
	HWREG(INTC_CONTROL) |= (1U << 0);
}

void irqHandlerGpio1A(void) {
	if(HWREG(GPIO1_IRQSTATUS_0) & BUTTON_01_MASK) {
		/* Clear the status of the interrupt flags */
		HWREG(GPIO1_IRQSTATUS_0) = BUTTON_01_MASK; 
		setBlinkMode(1);
	}
}

void irqHandlerGpio1B(void) {
	if(HWREG(GPIO1_IRQSTATUS_1) & BUTTON_02_MASK) {
		/* Clear the status of the interrupt flags */
		HWREG(GPIO1_IRQSTATUS_1) = BUTTON_02_MASK; 
		setBlinkMode(2);
	}
}

void irqHandlerGpio2A(void) {
	if(HWREG(GPIO2_IRQSTATUS_0) & BUTTON_03_MASK) {
		/* Clear the status of the interrupt flags */
		HWREG(GPIO2_IRQSTATUS_0) = BUTTON_03_MASK; 
		setBlinkMode(3);
	}
}

void irqHandlerGpio2B(void) {
	
}

// =============================================================================
// PRIVATE FUNCTIONS IMPLEMENTATIONS
// =============================================================================

void disableWdt(void) {
	HWREG(WDT_WSPR) = 0xAAAA;
	while((HWREG(WDT_WWPS) & (1<<4)));
	
	HWREG(WDT_WSPR) = 0x5555;
	while((HWREG(WDT_WWPS) & (1<<4)));
}

void intConfig() {
	/* Enable interface clock autogating. */
	HWREG(INTC_SYSCONFIG) |= (1U << 0);

	/* Enable interface clock autogating. */
	HWREG(INTC_IDLE) |= (1U << 1) | (1U << 0);
}