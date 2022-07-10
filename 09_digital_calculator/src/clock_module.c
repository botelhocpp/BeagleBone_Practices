/*
 * =====================================================================================
 *
 *       Filename:  control_base.c
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

#include "clock_module.h"
#include "hw_types.h"
#include "soc_AM335x.h"


static bool ckmCheckValidModule(CKM_MODULE_REG offset){
	if((offset > CKM_PER_CLK_24MHZ_CLKSTCTRL)){
      		return(false);
   	}
   	return(true);
}

void ckmSetCLKModuleRegister(CKM_MODULE base, CKM_MODULE_REG offset, unsigned int value){
   	if(ckmCheckValidModule(offset)){
						unsigned int addr_temp = base + offset; // clock base + offset, TRM 2.1 & 8.1.12.1
						HWREG(addr_temp) |= value;
  	}
}

unsigned int ckmGetCLKModuleRegister(CKM_MODULE base, CKM_MODULE_REG offset){
   	if(ckmCheckValidModule(offset)){
						unsigned int addr_temp = base + offset; // clock base + offset, TRM 2.1 & 8.1.12.1
      			return(HWREG(addr_temp));
   	}
   	return(0);
}

