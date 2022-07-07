/*
 * =====================================================================================
 *
 *       Filename:  control_module.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/02/2017 19:43:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include "control_module.h"
#include "soc_AM335x.h"
#include "hw_types.h" 

void cmSetCtrlModule(CONTROL_MODULE offset, unsigned int value){
   	HWREG(SOC_CONTROL_REGS + offset) |= value;
}

unsigned int cmGetCtrlModule(CONTROL_MODULE offset){
   	return(HWREG(SOC_CONTROL_REGS + offset));
}

