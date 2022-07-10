/*
 * =====================================================================================
 *
 *       Filename:  cpu.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/05/2018 19:53:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */


#include "cpu.h"

/*****************************************************************************
**                   FUNCTION DEFINITIONS
******************************************************************************/
/**
 * \brief     This API can be used to switch from user mode to privileged mode
 *            The priviledge mode will be system mode. System mode will share 
 *            the same resources as user mode, but with privileges.
 *
 * \param     None.
 *
 * \return    None.
 *
 * Note : All the access to system configuration which needs privileged access
 *        can be done after calling this API.
 **/
void CPUSwitchToPrivilegedMode(void){
    __asm("    SWI   458752");
}

/**
 * \brief     This API can be used to switch from any previleged mode of ARM to 
 *            user mode. After this API is called, the program will continue
 *            to operate in non-privileged mode, until any exception occurs.
 *            After the exception is serviced, execution will continue in user
 *            mode.
 *
 * \param     None.
 *
 * \return    None.
 *
 * Note : All the access to system configuration which needs privileged access
 *        shall be done before this API is called.
 **/
void CPUSwitchToUserMode(void){
    __asm("    mrs     r0, CPSR\n\t"
        "    bic     r0, #0x0F\n\t"
        "    orr     r0, #0x10\n\t "
        "    msr     CPSR, r0");
}

/**
 * \brief     This API is called when the CPU is aborted or during execution
 *            of any undefined instruction. Both IRQ and FIQ will be disabled
 *            when the CPU gets an abort and calls this API. 
 *
 * \param     None.
 *
 * \return    None.
 *
 * Note : The user can perform error handling such as an immediate reset 
 *        inside this API if required.
 **/
void CPUAbortHandler(void){
    ;  /* Perform Nothing */
}

/*
**
** Wrapper function for the IRQ status
**
*/
unsigned int CPUIntStatus(void){
    unsigned int stat;

    /* IRQ and FIQ in CPSR */
    __asm("    mrs     r0, CPSR\n\t"
				"    and     %[result], r0, #0xC0" : [result] "=r" (stat));
    
    return stat;
}

/*
**
** Wrapper function for the IRQ disable function
**
*/
void CPUirqd(void){
    /* Disable IRQ in CPSR */
    __asm("    dsb    \n\t"
        "    mrs     r0, CPSR\n\t"
        "    orr     r0, #0x80\n\t"
        "    msr     CPSR, r0");
}

/*
**
** Wrapper function for the IRQ enable function
**
*/
void CPUirqe(void){
    /* Enable IRQ in CPSR */
    __asm("    dsb    \n\t"
        "    mrs     r0, CPSR\n\t"
        "    bic     r0, #0x80\n\t"
        "    msr     CPSR, r0");
}

/*
**
** Wrapper function for the FIQ disable function
**
*/
void CPUfiqd(void){
    /* Disable FIQ in CPSR */
    __asm("    dsb    \n\t"
        "    mrs     r0, CPSR\n\t"
        "    orr     r0, #0x40\n\t"
        "    msr     CPSR, r0");
}

/*
**
** Wrapper function for the FIQ enable function
**
*/
void CPUfiqe(void){
    /* Enable FIQ in CPSR */
    __asm("    dsb    \n\t"
        "    mrs     r0, CPSR\n\t"
        "    bic     r0, #0x40\n\t"
        "    msr     CPSR, r0");
}