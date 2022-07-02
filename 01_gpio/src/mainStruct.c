/**
 * @file mainStruct.c
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @brief Toggle the one embedded LED with a struct.
 * @version 1.0
 * @date 2022-07-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include  "stdint.h"
#include	"hw_types.h"
#include	"soc_AM335x.h"

/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS
*****************************************************************************/
#define TIME													1000000
#define TOGGLE          										     (0x01u)

#define CM_PER_GPIO1											0xAC
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE   				          (0x2u)
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK   				(0x00040000u)

#define CM_conf_gpmc_ben1      	 								0x0878
#define CM_conf_gpmc_a5         									0x0854

#define GPIO_OE                 									0x134
#define GPIO_CLEARDATAOUT       									0x190
#define GPIO_SETDATAOUT         									0x194

typedef struct {
	uint32_t REVISION;                 //0x0000 a 0x0003
	uint32_t RESERVED_0[3];            //0x0004 a 0x000F
	uint32_t SYSCONFIG;                //0x0010 a 0x0013
	uint32_t RESERVED_1[3];            //0x0014 a 0x001F
	uint32_t EOI;                      //0x0020 a 0x0023
	uint32_t IRQSTATUS_RAW_0;
	uint32_t IRQSTATUS_RAW_1;
	uint32_t IRQSTATUS_0;
	uint32_t IRQSTATUS_1;
	uint32_t IRQSTATUS_SET_0;
	uint32_t IRQSTATUS_SET_1;
	uint32_t IRQSTATUS_CLR_0;
	uint32_t IRQSTATUS_CLR_1;
	uint32_t IRQWAKEN_0;
	uint32_t IRQWAKEN_1;               //0x0048 a 0x004B
	uint32_t RESERVED_2[50];           //0x004C a 0x0113
	uint32_t SYSSTATUS;                //0x0114 a 0x0117
	uint32_t RESERVED_3[6];            //0x0118 a 0x012F
	uint32_t CTRL;                     //0x0130 a 0x0133
	uint32_t OE;
	uint32_t DATAIN;
	uint32_t DATAOUT;
	uint32_t LEVELDETECT0;
	uint32_t LEVELDETECT1;
	uint32_t RISINGDETECT;
	uint32_t FALLINGDETECT;
	uint32_t DEBOUNCENABLE;
	uint32_t DEBOUNCINGTIME;           //0x0154 a 0x0157
	uint32_t RESERVED_4[14];           //0x0158 a 0x018F
	uint32_t CLEARDATAOUT;             //0x0190 a 0x0193
	uint32_t SETDATAOUT;
} GPIO_t;

#define GPIO1 ((GPIO_t *) SOC_GPIO_1_REGS)

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
  
    	/* Change the state of the green LED. */
  	while (1){
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
	 *  configure clock GPIO in clock module
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1) |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;
	
	/*-----------------------------------------------------------------------------
	 * configure mux pin in control module
	 *-----------------------------------------------------------------------------*/
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a5) |= 7;
 
	/*-----------------------------------------------------------------------------
	 *  set pin direction 
	 *-----------------------------------------------------------------------------*/
	GPIO1->OE &= ~(1 << 21);
		
}/* -----  end of function ledInit  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledToggle
 *  Description:  
 * =====================================================================================
 */
void ledToggle( ){
		
		flagBlink ^= TOGGLE;

		if(flagBlink)
			// HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1<<28);
			GPIO1->SETDATAOUT |= (1 << 21);
		else
			// HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = (1<<28);
			GPIO1->CLEARDATAOUT |= (1 << 21);

}/* -----  end of function ledToggle  ----- */










