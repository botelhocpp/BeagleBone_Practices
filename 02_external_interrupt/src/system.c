#include "system.h"
#include "drivers.h"

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
	/* Obtain the current interrupt number */
	unsigned int irq_number = HWREG(INTC_SIR_IRQ) & 0x7F;
	
	// Base in IRQ number call a specific ISR
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

	/* Acknowledge IRQ and enable new IRQ generation */
	HWREG(INTC_CONTROL) |= (1U << 0);
}

void irqHandlerGpio1A(void) {
	// Checks if the generated IRQ came from the button 01
	if(HWREG(GPIO1_IRQSTATUS_0) & BUTTON_01_MASK) {
		/* Clear the status of the interrupt flags */
		HWREG(GPIO1_IRQSTATUS_0) = BUTTON_01_MASK; 
		// Change the blink sequence.
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

	/* Enable auto-idle mode for the Functional clock. */
	HWREG(INTC_IDLE) |= (1U << 1) | (1U << 0);
}