/**
 * @file mainButtonSimpleStruct.c
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @brief Control the embedded LEDs with a button, via struct.
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
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE   				          0x2u
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK   				(0x00040000u)

#define CM_PER_GPIO2											0xB0
#define CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE   				          0x2u
#define CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK   				(0x00040000u)

#define CM_conf_gpmc_a5         									0x0854
#define CM_conf_gpmc_a6         									0x0858
#define CM_conf_gpmc_a7         									0x085C
#define CM_conf_gpmc_a8         									0x0860
#define CM_conf_gpmc_ad12										0x0830	  //GPIO1_12

#define GPIO_DATAIN         										0x138
#define GPIO_DATAOUT        										0x13C
#define GPIO_OE                 									0x134
#define GPIO_CLEARDATAOUT       									0x190
#define GPIO_SETDATAOUT         									0x194

#define PIN_AS_GPIO         										(0b111U << 0)
#define PIN_AS_RECEIVER         									(0b1U << 5)

#define LED_01													(1U << 21)
#define LED_02													(1U << 22)
#define LED_03													(1U << 23)
#define LED_04													(1U << 24)
#define BTN_01													(1U << 12)  //GPIO1_12

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
} GPIO_Type;

typedef struct {
	uint32_t RESERVED_0[42]; // 0x0000 a 0x00AB
	uint32_t GPIO1_CLKCTRL; // 0x00AC a 0x00BF
	uint32_t GPIO2_CLKCTRL;
	uint32_t GPIO3_CLKCTRL;
} CM_PER_Type;

#define CM_PER ((CM_PER_Type *) SOC_CM_PER_REGS)

#define GPIO1 ((GPIO_Type *) SOC_GPIO_1_REGS)
#define GPIO2 ((GPIO_Type *) SOC_GPIO_2_REGS)
#define GPIO3 ((GPIO_Type *) SOC_GPIO_3_REGS)

unsigned int flagBlink;

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void delay();
static void ledInit();
static void ledButton();

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
    		ledButton();
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
	CM_PER->GPIO1_CLKCTRL |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;
	
	/*-----------------------------------------------------------------------------
	 * configure mux pin in control module
	 *-----------------------------------------------------------------------------*/
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a5) |= PIN_AS_GPIO;
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a6) |= PIN_AS_GPIO;
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a7) |= PIN_AS_GPIO;
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a8) |= PIN_AS_GPIO;
   	HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_ad12) |= PIN_AS_RECEIVER | PIN_AS_GPIO;
 
	/*-----------------------------------------------------------------------------
	 *  set pin direction 
	 *-----------------------------------------------------------------------------*/
	GPIO1->OE &= ~LED_01 & ~LED_02 & ~LED_03 & ~LED_04;
	GPIO1->OE |= BTN_01;
		
}/* -----  end of function ledInit  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledToggle
 *  Description:  
 * =====================================================================================
 */
void ledButton( ){
		if(GPIO1->DATAIN & BTN_01)
			GPIO1->SETDATAOUT |= LED_01 | LED_02 | LED_03 | LED_04;
		else
			GPIO1->CLEARDATAOUT |= LED_01 | LED_02 | LED_03 | LED_04;

}/* -----  end of function ledToggle  ----- */










