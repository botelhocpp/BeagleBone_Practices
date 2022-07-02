/**
 * @file mainDiminutive.c
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @brief Blink the 4 embedded LEDs and a external LED.
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

// CLOCK MODULE OFFSET FOR GPIO1
#define CM_PER_GPIO1											0xAC

// CLOCK MODULE CONTROL BITS
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE   				          0x2u
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK   				(0x00040000u)

// CLOCK MODULE 
#define CM_GPIO1 		(*((volatile unsigned int *) (SOC_CM_PER_REGS + CM_PER_GPIO1)))

// PINS OFFSETS 
#define CM_CONF_GPMC_BEN1      	 								0x0878
#define CM_CONF_GPMC_A5         									0x0854
#define CM_CONF_GPMC_A6         									0x0858
#define CM_CONF_GPMC_A7         									0x085C
#define CM_CONF_GPMC_A8         									0x0860

// REGISTERS OFFSETS
#define GPIO_OE                 									0x134
#define GPIO_CLEARDATAOUT       									0x190
#define GPIO_SETDATAOUT         									0x194

// LED MASKS
#define LED_01													(1U << 21)
#define LED_02													(1U << 22)
#define LED_03													(1U << 23)
#define LED_04													(1U << 24)
#define LED_EXT												(1U << 28)

// PIN CONTROL REGISTERS
#define CTRL_REG_BEN1	(*((volatile unsigned int *) (SOC_CONTROL_REGS + CM_CONF_GPMC_BEN1)))
#define CTRL_REG_A5		(*((volatile unsigned int *) (SOC_CONTROL_REGS + CM_CONF_GPMC_A5)))
#define CTRL_REG_A6		(*((volatile unsigned int *) (SOC_CONTROL_REGS + CM_CONF_GPMC_A6)))
#define CTRL_REG_A7		(*((volatile unsigned int *) (SOC_CONTROL_REGS + CM_CONF_GPMC_A7)))
#define CTRL_REG_A8		(*((volatile unsigned int *) (SOC_CONTROL_REGS + CM_CONF_GPMC_A8)))

// GPIO1 REGISTERS
#define GPIO1_OE		(*((volatile unsigned int *) (SOC_GPIO_1_REGS + GPIO_OE)))
#define GPIO1_SETDATA	(*((volatile unsigned int *) (SOC_GPIO_1_REGS + GPIO_SETDATAOUT)))
#define GPIO1_CLEARDATA	(*((volatile unsigned int *) (SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT)))

unsigned int flagBlink;

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void delay(void);
static void ledInit(void);
static void ledToggle(void);


/*****************************************************************************
**                		MAIN FUNCTION
*****************************************************************************/
int _main(void){
  	flagBlink = 0;
	// INIT ALL LEDS
  	ledInit();
  
  	while (1){
    		// CHANGE THE STATE OF THE LEDS
    		ledToggle();
		delay();
	}

	return(0);
} 


/*****************************************************************************
**                INTERNAL FUNCTION IMPLEMENTATION
*****************************************************************************/
static void delay(){
	volatile unsigned int ra;
	for(ra = 0; ra < TIME; ra ++);
}

void ledInit( ){
	// ENABLE THE GPIO1 MODULE PERIPHERAL AND OPTIONAL FUNCTIONAL CLOCK FOR IT.
	CM_GPIO1 |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;
	
	// SET THE GPIO FUNCTION FOR THE PINS IN THE MUX.
   	CTRL_REG_BEN1 |= 7;
	CTRL_REG_A5 |= 7;
	CTRL_REG_A6 |= 7;
	CTRL_REG_A7 |= 7;
	CTRL_REG_A8 |= 7;
 
	// SET OUTPUT DIRECTION FOR THE USED PINS.
	GPIO1_OE &= ~LED_EXT & ~LED_01 & ~LED_02 & ~LED_03 & ~LED_04;
		
}

void ledToggle( ){
		// NEXT LED STATE
		flagBlink ^= TOGGLE;

		if(flagBlink)
			// IF IT HAVE TO BE ON, SET DATA FOR ALL LEDS
			GPIO1_SETDATA |= LED_EXT | LED_01 | LED_02 | LED_03 | LED_04;
		else
			// IF IT HAVE TO BE OFF, CLEAR DATA FOR ALL LEDS
			GPIO1_CLEARDATA |= LED_EXT | LED_01 | LED_02 | LED_03 | LED_04;

}
