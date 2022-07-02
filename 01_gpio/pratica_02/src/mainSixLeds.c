/**
 * @file mainSixLeds.c
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @brief Toggle the 4 embedded LEDs and the 2 external LEDs.
 * @version 1.0
 * @date 2022-07-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include	"hw_types.h"
#include	"soc_AM335x.h"

/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS
*****************************************************************************/
#define TIME													1000000
#define TOGGLE          										     (0x01u)

#define CM_PER_GPIO1											0xAC
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE   				          0x2u
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK   				(0x00040000u)

#define CM_PER_GPIO2											0xB0
#define CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE   				          0x2u
#define CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK   				(0x00040000u)

#define CM_conf_gpmc_ben1      	 								0x0878	//GPIO1_28
#define CM_conf_gpmc_a5         									0x0854
#define CM_conf_gpmc_a6         									0x0858
#define CM_conf_gpmc_a7         									0x085C
#define CM_conf_gpmc_a8         									0x0860
#define CM_conf_gpmc_clk											0x088C	  //GPIO2_1
#define CM_conf_gpmc_ad12										0x0830	  //GPIO1_12

#define GPIO_DATAIN         										0x138
#define GPIO_DATAOUT        										0x13C
#define GPIO_OE                 									0x134
#define GPIO_CLEARDATAOUT       									0x190
#define GPIO_SETDATAOUT         									0x194

#define LED_01													(1U << 21)
#define LED_02													(1U << 22)
#define LED_03													(1U << 23)
#define LED_04													(1U << 24)
#define LED_EXT_01												(1U << 28)  //GPIO1_28
#define LED_EXT_02												(1U << 1)   //GPIO2_1
#define BTN_01													(1U << 12)  //GPIO1_12

unsigned int flagBlink;

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void delay();
static void ledInit();
static void ledToggle();

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int _main(void){

	flagBlink = 0;	//init flag
  	
	/* Configure the green LED control pin. */
  	ledInit();
  
  	while (1){
    	/* Change the state of the green LED. */
    		ledToggle();
		delay();
	}

	return(0);
} /* ----------  end of function main  ---------- */


/*FUNCTION*-------------------------------------------------------
*
* Function Name : Delay
* Comments      :
*END*-----------------------------------------------------------*/
static void delay(){
	volatile unsigned int ra;
	for(ra = 0; ra < TIME; ra ++);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledInit
 *  Description:  
 * =====================================================================================
 */
void ledInit( ){
	/*-----------------------------------------------------------------------------
	 *  configure clock GPIO1 in clock module
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1) |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;
	HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2) |= CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK | CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE;
	
	/*-----------------------------------------------------------------------------
	 * configure mux pin in control module
	 *-----------------------------------------------------------------------------*/
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a5) |= 7;
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a6) |= 7;
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a7) |= 7;
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a8) |= 7;
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_ad12) |= 7;
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_ben1) |= 7;
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_clk) |= 7;
 
	/*-----------------------------------------------------------------------------
	 *  set pin direction 
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_GPIO_1_REGS + GPIO_OE) |= BTN_01;
	HWREG(SOC_GPIO_1_REGS + GPIO_OE) &= ~LED_EXT_01 & ~LED_01 & ~LED_02 & ~LED_03 & ~LED_04;
	HWREG(SOC_GPIO_2_REGS + GPIO_OE) &= ~LED_EXT_02;
		
}/* -----  end of function ledInit  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledToggle
 *  Description:  
 * =====================================================================================
 */
void ledToggle( ){
		
		flagBlink ^= TOGGLE;

		if(flagBlink) {
			HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) |= LED_EXT_01 | LED_01 | LED_02 | LED_03 | LED_04;
			HWREG(SOC_GPIO_2_REGS + GPIO_SETDATAOUT) |= LED_EXT_02;
		}
		else {
			HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) |= LED_EXT_01 | LED_01 | LED_02 | LED_03 | LED_04;
			HWREG(SOC_GPIO_2_REGS + GPIO_CLEARDATAOUT) |= LED_EXT_02;
		}
}/* -----  end of function ledToggle  ----- */










