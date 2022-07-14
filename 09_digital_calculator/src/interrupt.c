/*
 * =====================================================================================
 *
 *       Filename:  interrupt.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/05/2018 17:47:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include "gpio.h"
#include "interrupt.h"
#include "cpu.h"

/******************************************************************************
**                INTERNAL MACRO DEFINITIONS
******************************************************************************/
#define REG_IDX_SHIFT                  (0x05)
#define REG_BIT_MASK                   (0x1F)
#define NUM_INTERRUPTS                 (128u)

/**************** *************************************************************
**                 STATIC VARIABLE DEFINITIONS
******************************************************************************/
void (*fnRAMVectors[NUM_INTERRUPTS])(void);

/******************************************************************************
**                STATIC FUNCTION DECLARATIONS
******************************************************************************/
static void IntDefaultHandler(void);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name: IntRegister 
 *  Description: The Default Interrupt Handler. This is the default interrupt handler 
 *  for all interrupts. It simply returns without performing any operation.
 * =====================================================================================
 */
static void IntDefaultHandler(void){
    /* Go Back. Nothing to be done */
    ;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: IntRegister 
 *  Description: Registers an interrupt Handler in the interrupt vector table for
 *           system interrupts. 
 * 
 * \param    intrNum - Interrupt Number
 * \param    fnHandler - Function pointer to the ISR
 * 
 * Note: When the interrupt occurs for the sytem interrupt number indicated,
 * the control goes to the ISR given as the parameter.
 * 
 * \return      None.
 * =====================================================================================
 */
void IntRegister(unsigned int intrNum, void (*fnHandler)(void)){
    /* Assign ISR */
    fnRAMVectors[intrNum] = fnHandler;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: IntUnRegister 
 *  Description: Unregisters an interrupt
 * 
 * \param   intrNum - Interrupt Number
 * 
 * Note: Once an interrupt is unregistered it will enter infinite loop once
 * an interrupt occurs
 * 
 * \return      None.
* =====================================================================================
 */
void IntUnRegister(unsigned int intrNum){
    /* Assign default ISR */
    fnRAMVectors[intrNum] = IntDefaultHandler;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  IntAINTCInit
 *  Description:  This API is used to initialize the interrupt controller. This API  
 *          shall be called before using the interrupt controller. 
 *
 * \param   None
 * 
 * \return  None.
 * =====================================================================================
 */
void IntAINTCInit(void){
    /* Reset the ARM interrupt controller */
    HWREG(SOC_AINTC_REGS + INTC_SYSCONFIG) = INTC_SYSCONFIG_SOFTRESET;
 
    /* Wait for the reset to complete */
    while((HWREG(SOC_AINTC_REGS + INTC_SYSSTATUS) 
          & INTC_SYSSTATUS_RESETDONE) != INTC_SYSSTATUS_RESETDONE);    
  
    /* Enable any interrupt generation by setting priority threshold */ 
    HWREG(SOC_AINTC_REGS + INTC_THRESHOLD) = 
                                       INTC_THRESHOLD_PRIORITYTHRESHOLD;

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: IntPrioritySet 
 *  Description: This API assigns a priority to an interrupt and routes it to
 *          either IRQ or to FIQ. Priority 0 is the highest priority level
 *          Among the host interrupts, FIQ has more priority than IRQ.
 *
 * \param   intrNum  - Interrupt number
 * \param   priority - Interrupt priority level
 * \param   hostIntRoute - The host interrupt IRQ/FIQ to which the interrupt
 *                         is to be routed.
 *     'priority' can take any value from 0 to 127, 0 being the highest and
 *     127 being the lowest priority.              
 *
 *     'hostIntRoute' can take one of the following values \n
 *             AINTC_HOSTINT_ROUTE_IRQ - To route the interrupt to IRQ \n
 *             AINTC_HOSTINT_ROUTE_FIQ - To route the interrupt to FIQ
 *
 * \return  None.
 * =====================================================================================
 */
void IntPrioritySet(unsigned int intrNum, unsigned int priority,
                    unsigned int hostIntRoute){
    HWREG(SOC_AINTC_REGS + INTC_ILR(intrNum)) =
                                 ((priority << INTC_ILR_PRIORITY_SHIFT)
                                   & INTC_ILR_PRIORITY)
                                 | hostIntRoute ;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: IntSystemEnable  
 *  Description: This API enables the system interrupt in AINTC. However, for 
 *          the interrupt generation, make sure that the interrupt is 
 *          enabled at the peripheral level also. 
 *
 * \param   intrNum  - Interrupt number
 *
 * \return  None.
 * =====================================================================================
 */
void IntSystemEnable(unsigned int intrNum){
    /* Disable the system interrupt in the corresponding MIR_CLEAR register */
    HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR(intrNum >> REG_IDX_SHIFT))
                                   |= (0x01 << (intrNum & REG_BIT_MASK));
}

/* 
* ===  FUNCTION  ======================================================================
*         Name: IntSystemDisable 
*  Description: This API disables the system interrupt in AINTC. 
 *
 * \param   intrNum  - Interrupt number
 *
 * \return  None.
 *
 * =====================================================================================
*/
void IntSystemDisable(unsigned int intrNum){
    /* Enable the system interrupt in the corresponding MIR_SET register */
    HWREG(SOC_AINTC_REGS + INTC_MIR_SET(intrNum >> REG_IDX_SHIFT)) 
                                   = (0x01 << (intrNum & REG_BIT_MASK));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  IntMasterIRQEnable
 *  Description: Enables the processor IRQ only in CPSR. Makes the processor to respond 
 *  to IRQs.  This does not affect the set of interrupts enabled/disabled in the AINTC.
 *
 * \param    None
 *
 * \return   None
 *
 *  Note: This function call shall be done only in previleged mode of ARM 
 * =====================================================================================
 */
void IntMasterIRQEnable( ){
	  /* Enable IRQ in CPSR.*/
    CPUirqe();
}/* -----  end of function IntMasterIRQEnable  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  IntMasterIRQDisable
 *  Description:  Enables the processor IRQ only in CPSR. Makes the processor to respond 
 *  to IRQs.  This does not affect the set of interrupts enabled/disabled in the AINTC.
 *
 * \param    None
 *
 * \return   None
 *
 *  Note: This function call shall be done only in previleged mode of ARM 
 * =====================================================================================
 */
void IntMasterIRQDisable( ){
		/* Enable IRQ in CPSR.*/
    CPUirqd();

}/* -----  end of function IntMasterIRQDisable  ----- */

void IntIrqHandler(void) { 

     /* Obtain the current interrupt number */
	unsigned int irq_number = HWREG(SOC_AINTC_REGS + INTC_SIR_IRQ) & 0x7F;
	
    /* Obtain the ISR for the curret interrupt */
    void (*irq_handler)() = fnRAMVectors[irq_number];

	/* Call the ISR to handle the interrupt */
	(*irq_handler)();

	/* Acknowledge IRQ and enable new IRQ generation */
	HWREG(SOC_AINTC_REGS + INTC_CONTROL) |= (1U << 0);
}

void IntDisableWatchdog(void) {
	HWREG(SOC_WDT_1_REGS + WDT_WSPR) = 0xAAAA;
	while((HWREG(SOC_WDT_1_REGS + WDT_WWPS) & (1<<4)));
	
	HWREG(SOC_WDT_1_REGS + WDT_WSPR) = 0x5555;
	while((HWREG(SOC_WDT_1_REGS + WDT_WWPS) & (1<<4)));
}
