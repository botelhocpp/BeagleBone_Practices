/*
 * =====================================================================================
 *
 *       Filename:  timer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31/05/2018 19:30:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include "timer.h"
#include "uart.h"

/******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/

/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
static volatile unsigned int flagIsr = false;

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerWritePostedStatusGet
* Comments      : Read the status of Write Posted Status register.
*
* \param   baseAdd       Base Address of the DMTimer Module Register.
*
* \return  This API returns the status of TWPS register.
*END*-----------------------------------------------------------*/
static unsigned int DMTimerWritePostedStatusGet(unsigned int baseAdd) {
    /* Return the status of TWPS register */
    return (HWREG(baseAdd + DMTIMER_TWPS));
}


/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerWaitForWrite
* Comments      : This macro will check for write POSTED status
*
* \param   reg          Register whose status has to be checked
*
*    'reg' can take the following values \n
*    DMTIMER_WRITE_POST_TCLR - Timer Control register \n
*    DMTIMER_WRITE_POST_TCRR - Timer Counter register \n
*    DMTIMER_WRITE_POST_TLDR - Timer Load register \n
*    DMTIMER_WRITE_POST_TTGR - Timer Trigger register \n
*    DMTIMER_WRITE_POST_TMAR - Timer Match register \n
*
*END*-----------------------------------------------------------*/
#define DMTimerWaitForWrite(reg, baseAdd)   \
            if(HWREG(baseAdd + DMTIMER_TSICR) & DMTIMER_TSICR_POSTED)\
            while((reg & DMTimerWritePostedStatusGet(baseAdd)));

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerReloadSet
* Comments      : Set the reload count value in the timer load register.
*
* \param   baseAdd       Base Address of the DMTimer Module Register.
* \param   reload        The reload count value of the timer.
*
* \return  None.
*
* \note:   It is recommended to not use reload value as 0xFFFFFFFF as it can 
*          lead to undesired results.
*
*END*-----------------------------------------------------------*/
static void DMTimerReloadSet(unsigned int baseAdd, unsigned int reload) {
    /* Wait for previous write to complete */
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TLDR, baseAdd);

    /* Load the register with the re-load value */
    HWREG(baseAdd + DMTIMER_TLDR) = reload;
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerModeConfigure
* Comments      : This API will configure the timer in combinations 
* of One Shot timer' and 'Compare' Mode or 'Auto-reload timer' 
* and 'Compare' Mode.  
*
* \param   baseAdd      Base Address of the DMTimer Module Register.
* \param   timerMode    Mode for enabling the timer.
*
* 'timerMode' can take the following values \n
*    DMTIMER_ONESHOT_CMP_ENABLE - One shot and compare mode enabled \n
*    DMTIMER_ONESHOT_NOCMP_ENABLE - One shot enabled, compare mode disabled \n
*    DMTIMER_AUTORLD_CMP_ENABLE - Auto-reload and compare mode enabled \n
*    DMTIMER_AUTORLD_NOCMP_ENABLE - Auto-reload enabled, compare mode 
*                                   disabled \n
*
* \return  None.
*
*END*-----------------------------------------------------------*/
static void DMTimerModeConfigure(unsigned int baseAdd, unsigned int timerMode) {
    /* Wait for previous write to complete */
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCLR, baseAdd);

    /* Clear the AR and CE field of TCLR */
    HWREG(baseAdd + DMTIMER_TCLR) &= ~(DMTIMER_TCLR_AR | DMTIMER_TCLR_CE);

    /* Wait for previous write to complete */
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCLR, baseAdd);

    /* Set the timer mode in TCLR register */
    HWREG(baseAdd + DMTIMER_TCLR) |= (timerMode & (DMTIMER_TCLR_AR | 
                                                   DMTIMER_TCLR_CE));
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerIntStatusClear
* Comments      : Clear the status of interrupt events.
* \param   baseAdd       Base Address of the DMTimer Module Register.
* \param   intFlags      Variable used to clear the events.
*
* 'intFlags' can take the following values \n
*    DMTIMER_INT_TCAR_IT_FLAG - Clear IRQ status for capture \n
*    DMTIMER_INT_OVF_IT_FLAG - Clear IRQ status for overflow \n
*    DMTIMER_INT_MAT_IT_FLAG - Clear IRQ status for match \n
*
* \return  None.
*
*END*-----------------------------------------------------------*/
static void DMTimerIntStatusClear(unsigned int baseAdd, unsigned int intFlags) {

    /* Clear the interrupt status from IRQSTATUS register */
    HWREG(baseAdd + DMTIMER_IRQSTATUS) = (intFlags & 
                                         (DMTIMER_IRQSTATUS_TCAR_IT_FLAG | 
                                          DMTIMER_IRQSTATUS_OVF_IT_FLAG | 
                                          DMTIMER_IRQSTATUS_MAT_IT_FLAG));
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerModuleClkConfig
* Comments      : configure timer 7 module clock. 
*
* \param   
*
*END*-----------------------------------------------------------*/
void DMTimerModuleClkConfig(void){

	ckmSetCLKModuleRegister(SOC_CM_PER_REGS, CM_PER_L3S_CLKSTCTRL, CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
     CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP){};

	ckmSetCLKModuleRegister(SOC_CM_PER_REGS, CM_PER_L3_CLKSTCTRL, CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
     CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP){};

	ckmSetCLKModuleRegister(SOC_CM_PER_REGS, CM_PER_L3_INSTR_CLKCTRL, CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
                               CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) !=
                                   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE){};
		
	ckmSetCLKModuleRegister(SOC_CM_PER_REGS, CM_PER_L3_CLKCTRL, CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
        CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE){};
		
	ckmSetCLKModuleRegister(SOC_CM_PER_REGS, CM_PER_OCPWP_L3_CLKSTCTRL, CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    while((HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
                              CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) !=
                                CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP){};

	ckmSetCLKModuleRegister(SOC_CM_PER_REGS, CM_PER_L4LS_CLKSTCTRL, CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) !=
                               CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP){};

	ckmSetCLKModuleRegister(SOC_CM_PER_REGS, CM_PER_L4LS_CLKCTRL, CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) &
      CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE){};

    /* Select the clock source for the Timer7 instance. */
    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER7_CLK) &=
          ~(CM_DPLL_CLKSEL_TIMER7_CLK_CLKSEL);

    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER7_CLK) |=
          CM_DPLL_CLKSEL_TIMER7_CLK_CLKSEL_CLK_M_OSC;

    while((HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER7_CLK) &
           CM_DPLL_CLKSEL_TIMER7_CLK_CLKSEL) !=
           CM_DPLL_CLKSEL_TIMER7_CLK_CLKSEL_CLK_M_OSC){};
   
	ckmSetCLKModuleRegister(SOC_CM_PER_REGS, CM_PER_TIMER7_CLKCTRL, CM_PER_TIMER7_CLKCTRL_MODULEMODE_ENABLE);

    while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER7_CLKCTRL) &
    CM_PER_TIMER7_CLKCTRL_MODULEMODE) != CM_PER_TIMER7_CLKCTRL_MODULEMODE_ENABLE){};

    while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER7_CLKCTRL) & 
     CM_PER_TIMER7_CLKCTRL_IDLEST) != CM_PER_TIMER7_CLKCTRL_IDLEST_FUNC){};

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK)){};

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK |
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L4_GCLK)));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK |
            CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_TIMER7_GCLK)));

}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerCounterSet
* Comments      :Set/Write the Counter register with the counter value.
*
* \param   baseAdd       Base Address of the DMTimer Module Register.
* \param   counter       Count value for the timer.
*
* \return  None.
*
* \note    Value can be loaded into the counter register when the counter is 
*          stopped or when it is running.
*
*END*-----------------------------------------------------------*/
void DMTimerCounterSet(unsigned int baseAdd, unsigned int counter) {
    /* Wait for previous write to complete */
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCRR, baseAdd);

    /* Set the counter value */
    HWREG(baseAdd + DMTIMER_TCRR) = counter;
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerCounterGet
* Comments      : Get/Read the counter value from the counter register.
*
* \param   baseAdd       Base Address of the DMTimer Module Register.
*
* \return  This API returns the count value present in the DMTimer Counter
*          register.
*
* \note:   Value can be read from the counter register when the counter is
*          stopped or when it is running.
*END*-----------------------------------------------------------*/
unsigned int DMTimerCounterGet(unsigned int baseAdd){
    /* Wait for previous write to complete */
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCRR, baseAdd);

    /* Read the counter value from TCRR */
    return (HWREG(baseAdd + DMTIMER_TCRR));
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerEnable
* Comments      :This API will start the timer.  
* \param   baseAdd       Base Address of the DMTimer Module Register.
*
* \return  None.
*
* \note    The timer must be configured before it is started/enabled.
*
*END*-----------------------------------------------------------*/
void DMTimerEnable(unsigned int baseAdd){

    /* Wait for previous write to complete */
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCLR, baseAdd);

    /* Start the timer */
    HWREG(baseAdd + DMTIMER_TCLR) |= DMTIMER_TCLR_ST;
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerDisable
* Comments      :This API will stop the timer.  
* \param   baseAdd       Base Address of the DMTimer Module Register.
*
* \return  None.
*
*END*-----------------------------------------------------------*/
void DMTimerDisable(unsigned int baseAdd){

    /* Wait for previous write to complete */
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCLR, baseAdd);

    /* Stop the timer */
    HWREG(baseAdd + DMTIMER_TCLR) &= ~DMTIMER_TCLR_ST;
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerIntpEnable
* Comments      : Enable the DMTimer interrupts.
*
* \param   baseAdd       Base Address of the DMTimer Module Register.
* \param   intFlags      Variable used to enable the interrupts.
*
* 'intFlags' can take the following values \n
*    DMTIMER_INT_TCAR_EN_FLAG - IRQ enable for capture \n
*    DMTIMER_INT_OVF_EN_FLAG - IRQ enable for overflow \n
*    DMTIMER_INT_MAT_EN_FLAG - IRQ enable for match \n
*
* \return  None.
*
*END*-----------------------------------------------------------*/
void DMTimerIntEnable(unsigned int baseAdd, unsigned int intFlags) {

    /* Enable the DMTimer interrupts represented by intFlags */
    HWREG(baseAdd + DMTIMER_IRQENABLE_SET) = (intFlags & 
                                           (DMTIMER_IRQENABLE_SET_TCAR_EN_FLAG |
                                            DMTIMER_IRQENABLE_SET_OVF_EN_FLAG | 
                                            DMTIMER_IRQENABLE_SET_MAT_EN_FLAG));
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerReset
* Comments      : Reset the DMTimer module.
*
* \param   baseAdd       Base Address of the DMTimer Module Register.
*  Enable the DMTimer interrupts.
*
* \return  None.
*
*END*-----------------------------------------------------------*/
void DMTimerReset(unsigned int baseAdd){
    /* Reset the DMTimer module */
    HWREG(baseAdd + DMTIMER_TIOCP_CFG) |= DMTIMER_TIOCP_CFG_SOFTRESET;

    while(DMTIMER_TIOCP_CFG_SOFTRESET == (HWREG(baseAdd + DMTIMER_TIOCP_CFG)
                                          & DMTIMER_TIOCP_CFG_SOFTRESET));
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerIntpDisable
* Comments      : disable the DMTimer interrupts.
*
* \param   baseAdd       Base Address of the DMTimer Module Register.
* \param   intFlags      Variable used to disable the interrupts.
*
* 'intFlags' can take the following values \n
*    DMTIMER_INT_TCAR_EN_FLAG - IRQ disable for capture \n
*    DMTIMER_INT_OVF_EN_FLAG - IRQ disable for overflow \n
*    DMTIMER_INT_MAT_EN_FLAG - IRQ disable for match \n
* \return  None.
*
*END*-----------------------------------------------------------*/
void DMTimerIntDisable(unsigned int baseAdd, unsigned int intFlags) {

    /* Disable the DMTimer interrupts represented by intFlags */
    HWREG(baseAdd + DMTIMER_IRQENABLE_CLR) = (intFlags &
                                           (DMTIMER_IRQENABLE_CLR_TCAR_EN_FLAG |
                                            DMTIMER_IRQENABLE_CLR_OVF_EN_FLAG |
                                            DMTIMER_IRQENABLE_CLR_MAT_EN_FLAG));
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerSetup
* Comments      : Setup the timer for one-shot and compare mode.
*
* \param   
*
*END*-----------------------------------------------------------*/
void DMTimerSetUp(void){
#ifdef DELAY_USE_INTERRUPTS
     /* This function will enable clocks for the DMTimer7 instance */
    DMTimerModuleClkConfig();
 
    /* Enable the system interrupt */
    IntSystemEnable(SYS_INT_TINT7);

    DMTimerCounterSet(SOC_DMTIMER_7_REGS, 0);

    /* Configure the DMTimer for Auto-reload and compare mode */
    DMTimerModeConfigure(SOC_DMTIMER_7_REGS, DMTIMER_AUTORLD_NOCMP_ENABLE);
#else
    /* This function will enable clocks for the DMTimer7 instance */
    DMTimerModuleClkConfig();

    DMTimerModeConfigure(SOC_DMTIMER_7_REGS, DMTIMER_ONESHOT_NOCMP_ENABLE);
#endif
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : Delay
* Comments      :
*END*-----------------------------------------------------------*/
void Delay(unsigned int mSec){

#ifdef DELAY_USE_INTERRUPTS
    unsigned int countVal = TIMER_OVERFLOW - (mSec * TIMER_1MS_COUNT);

    DMTimerCounterSet(SOC_DMTIMER_7_REGS, countVal);

    flagIsr = false;

    /* Enable the DMTimer interrupts */
    DMTimerIntEnable(SOC_DMTIMER_7_REGS, DMTIMER_INT_OVF_EN_FLAG);

    /* Start the DMTimer */
    DMTimerEnable(SOC_DMTIMER_7_REGS);

    while(flagIsr == false);
 
    /* Disable the DMTimer interrupts */
    DMTimerIntDisable(SOC_DMTIMER_7_REGS, DMTIMER_INT_OVF_EN_FLAG);

#else
    
    while(mSec != 0){
        DMTimerCounterSet(SOC_DMTIMER_7_REGS, 0);
        DMTimerEnable(SOC_DMTIMER_7_REGS);
        while(DMTimerCounterGet(SOC_DMTIMER_7_REGS) < TIMER_1MS_COUNT);
        DMTimerDisable(SOC_DMTIMER_7_REGS);
        mSec--;
    }
#endif
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : DMTimerIsr
* Comments      : DMTimer interrupt service routine. This will send 
* a character to serial.
*
* \param   
*
*END*-----------------------------------------------------------*/
void DMTimerIsr(void){
 
    /* Clear the status of the interrupt flags */
    DMTimerIntStatusClear(SOC_DMTIMER_7_REGS, DMTIMER_INT_OVF_EN_FLAG);

    DMTimerDisable(SOC_DMTIMER_7_REGS);

    flagIsr = true;

}

