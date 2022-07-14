/**
 * @file gpio.c
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @author Francisco Helder (helderhdw@gmail.com)
 * @brief Provide functions for controlling the GPIO
 * peripheral through a simple structure.
 * @version 2.0
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "gpio.h"
#include "interrupt.h"

// =============================================================================
// PRIVATE VARIABLES
// =============================================================================

// Insert [pin, port], get CM_conf adress
static const CONTROL_MODULE GPIO_CTRL_MODULE_ARRAY[32][4] = {
    //p0                          //p1                          //p2                          //p3
   {CM_conf_mdio                 ,CM_conf_gpmc_ad0             ,CM_conf_gpmc_csn3            ,CM_conf_mii1_col       },//.0
   {CM_conf_mdc                  ,CM_conf_gpmc_ad1             ,CM_conf_gpmc_clk             ,CM_conf_mii1_crs       },//.1
   {CM_conf_spi0_sclk            ,CM_conf_gpmc_ad2             ,CM_conf_gpmc_advn_ale        ,CM_conf_mii1_rx_er     },//.2
   {CM_conf_spi0_d0              ,CM_conf_gpmc_ad3             ,CM_conf_gpmc_oen_ren         ,CM_conf_mii1_tx_en     },//.3
   {CM_conf_spi0_d1              ,CM_conf_gpmc_ad4             ,CM_conf_gpmc_wen             ,CM_conf_mii1_rx_dv     },//.4
   {CM_conf_spi0_cs0             ,CM_conf_gpmc_ad5             ,CM_conf_gpmc_ben0_cle        ,CM_conf_i2c0_sda       },//.5
   {CM_conf_spi0_cs1             ,CM_conf_gpmc_ad6             ,CM_conf_lcd_data0            ,CM_conf_i2c0_scl       },//.6
   {CM_conf_ecap0_in_pwm0_out    ,CM_conf_gpmc_ad7             ,CM_conf_lcd_data1            ,CM_conf_emu0           },//.7
   {CM_conf_lcd_data12           ,CM_conf_uart0_ctsn           ,CM_conf_lcd_data2            ,CM_conf_emu1           },//.8
   {CM_conf_lcd_data13           ,CM_conf_uart0_rtsn           ,CM_conf_lcd_data3            ,CM_conf_mii1_tx_clk    },//.9
   {CM_conf_lcd_data14           ,CM_conf_uart0_rxd            ,CM_conf_lcd_data4            ,CM_conf_mii1_rx_clk    },//.10
   {CM_conf_lcd_data15           ,CM_conf_uart0_txd            ,CM_conf_lcd_data5            ,-1                     },//.11
   {CM_conf_uart1_ctsn           ,CM_conf_gpmc_ad12            ,CM_conf_lcd_data6            ,-1                     },//.12
   {CM_conf_uart1_rtsn           ,CM_conf_gpmc_ad13            ,CM_conf_lcd_data7            ,CM_conf_usb1_drvvbus   },//.13
   {CM_conf_uart1_rxd            ,CM_conf_gpmc_ad14            ,CM_conf_lcd_data8            ,CM_conf_mcasp0_aclkx   },//.14
   {CM_conf_uart1_txd            ,CM_conf_gpmc_ad15            ,CM_conf_lcd_data9            ,CM_conf_mcasp0_fsx     },//.15
   {CM_conf_mii1_txd3            ,CM_conf_gpmc_a0              ,CM_conf_lcd_data10           ,CM_conf_mcasp0_axr0    },//.16
   {CM_conf_mii1_txd2            ,CM_conf_gpmc_a1              ,CM_conf_lcd_data11           ,CM_conf_mcasp0_ahclkr  },//.17
   {CM_conf_usb0_drvvbus         ,CM_conf_gpmc_a2              ,CM_conf_mii1_rxd3            ,CM_conf_mcasp0_aclkr   },//.18
   {CM_conf_xdma_event_intr0     ,CM_conf_gpmc_a3              ,CM_conf_mii1_rxd2            ,CM_conf_mcasp0_fsr     },//.19
   {CM_conf_xdma_event_intr1     ,CM_conf_gpmc_a4              ,CM_conf_mii1_rxd1            ,CM_conf_mcasp0_axr1    },//.20
   {CM_conf_mii1_txd1            ,CM_conf_gpmc_a5              ,CM_conf_mii1_rxd0            ,CM_conf_mcasp0_ahclkx  },//.21
   {CM_conf_gpmc_ad8             ,CM_conf_gpmc_a6              ,CM_conf_lcd_vsync            ,-1                     },//.22
   {CM_conf_gpmc_ad9             ,CM_conf_gpmc_a7              ,CM_conf_lcd_hsync            ,-1                     },//.23
   {-1                           ,CM_conf_gpmc_a8              ,CM_conf_lcd_pclk             ,-1                     },//.24
   {-1                           ,CM_conf_gpmc_a9              ,CM_conf_lcd_ac_bias_en       ,-1                     },//.25
   {CM_conf_gpmc_ad10            ,CM_conf_gpmc_a10             ,CM_conf_mmc0_dat3            ,-1                     },//.26
   {CM_conf_gpmc_ad11            ,CM_conf_gpmc_a11             ,CM_conf_mmc0_dat2            ,-1                     },//.27
   {CM_conf_mii1_txd0            ,CM_conf_gpmc_ben1            ,CM_conf_mmc0_dat1            ,-1                     },//.28
   {CM_conf_rmii1_ref_clk        ,CM_conf_gpmc_csn0            ,CM_conf_mmc0_dat0            ,-1                     },//.29
   {CM_conf_gpmc_wait0           ,CM_conf_gpmc_csn1            ,CM_conf_mmc0_clk             ,-1                     },//.30
   {CM_conf_gpmc_wpn             ,CM_conf_gpmc_csn2            ,CM_conf_mmc0_cmd             ,-1                     },//.31
};

// =============================================================================
// PRIVATE FUNCTION PROTOTYPES
// =============================================================================

bool gpioCheckValidPin(gpio_handle_t *pin);

bool gpioCheckValidPortPin(gpio_port port, uint8_t pin);

bool gpioCheckValidPort(gpio_port port);

bool gpioCheckValidpin_number(uint8_t pin);

bool gpioCheckValidDirection(pin_direction direction);

uint32_t gpioGetPort(gpio_port port);

// =============================================================================
// PUBLIC FUNCTION IMPLEMENTATION
// =============================================================================

void gpioFInitPin(gpio_handle_t *pin, pin_direction direction) {
	gpioInitModule(pin->port);
	gpioInitPin(pin);
	gpioSetPinDirection(pin, direction);
}

void gpioPInitPin(gpio_handle_t *pin, pin_direction direction) {
	gpioInitPin(pin);
	gpioSetPinDirection(pin, direction);
}

void gpioInitModule(gpio_port port) {
	if(!gpioCheckValidPort(port)) {
		return;
	}

	//enable functional clock & enable module
	uint32_t setting = (1U << 18) | (0b10U << 0);

	switch (port) {
		case GPIO0:
			// GPIO0 doesnt have a clock module register
			return;
			break;
		case GPIO1:
			ckmSetCLKModuleRegister(CKM_PER, CKM_PER_GPIO1_CLKCTRL, setting);
			while ((ckmGetCLKModuleRegister(CKM_PER, CKM_PER_GPIO1_CLKCTRL) & (0b11U << 16)) != 0);
			break;
		case GPIO2:
			ckmSetCLKModuleRegister(CKM_PER, CKM_PER_GPIO2_CLKCTRL, setting);
			while ((ckmGetCLKModuleRegister(CKM_PER, CKM_PER_GPIO2_CLKCTRL) & (0b11U << 16)) != 0);
			break;
		case GPIO3:
			ckmSetCLKModuleRegister(CKM_PER, CKM_PER_GPIO3_CLKCTRL, setting);
			while ((ckmGetCLKModuleRegister(CKM_PER, CKM_PER_GPIO3_CLKCTRL) & (0b11U << 16)) != 0);
			break;
		default:
			break;
	}
}

void gpioInitPin(gpio_handle_t *pin) {
	if(!gpioCheckValidPin(pin)) {
		return;
	}
	CONTROL_MODULE module = GPIO_CTRL_MODULE_ARRAY[pin->pin_number][pin->port]; 
	padSetMode(module, MODE_7);										
}

void gpioSetPinDirection(gpio_handle_t *pin, pin_direction direction) {
	if(!gpioCheckValidPin(pin) || !gpioCheckValidDirection(direction)){
		return;
   	}
	uint32_t gpio_base = gpioGetPort(pin->port);
	HWREG(gpio_base + GPIO_OE) &= ~(1U << pin->pin_number);
	HWREG(gpio_base + GPIO_OE) |= (direction << pin->pin_number);
}

pin_direction gpioGetPinDirection(gpio_handle_t *pin) {
	if(!gpioCheckValidPin(pin)) {
		return -1;
	} 
	uint32_t gpio_base = gpioGetPort(pin->port);
	return HWREG(gpio_base + GPIO_OE) & (1 << pin->pin_number);
}

void gpioSetPinValue(gpio_handle_t *pin, pin_level level) {
	if(!gpioCheckValidPin(pin) || (level > 1 || level < 0)) {
		return;
	} 
	uint32_t gpio_base = gpioGetPort(pin->port);
	HWREG(gpio_base + GPIO_DATAOUT) &= ~(1U << pin->pin_number);
	HWREG(gpio_base + GPIO_DATAOUT) |= (level << pin->pin_number);
}

void gpioTogglePinValue(gpio_handle_t *pin) {
	if(!gpioCheckValidPin(pin)) {
		return;
	} 
	uint32_t gpio_base = gpioGetPort(pin->port);
	HWREG(gpio_base + GPIO_DATAOUT) ^= (1U << pin->pin_number);
}

pin_level gpioGetPinValue(gpio_handle_t *pin) {
	if(!gpioCheckValidPin(pin)) {
		return -1;
	}
	uint32_t gpio_base = gpioGetPort(pin->port);
	return (HWREG(gpio_base + GPIO_DATAIN) >> pin->pin_number) & 1;
}

void gpioConfigPull(gpio_handle_t *pin, pin_pull pull_type) {
	if(!gpioCheckValidPin(pin) || pull_type > 1 || pull_type < 0) {
		return;
	}
	CONTROL_MODULE module_offset = GPIO_CTRL_MODULE_ARRAY[pin->pin_number][pin->port]; 
    uint32_t pin_module = cmGetCtrlModule(module_offset);
    pin_module &= ~(1U << 3) & ~(1U << 4);
    pin_module |= (pull_type << 4);
    cmSetCtrlModule(module_offset, pin_module);
}

// =============================================================================
// PRIVATE FUNCTION IMPLEMENTATION
// =============================================================================

bool gpioCheckValidPin(gpio_handle_t *pin){
   return gpioCheckValidPort(pin->port) && gpioCheckValidpin_number(pin->pin_number);
}

bool gpioCheckValidPortPin(gpio_port port, uint8_t pin){
   return gpioCheckValidPort(port) && gpioCheckValidpin_number(pin);
}

bool gpioCheckValidPort(gpio_port port){
   return ((port >= GPIO0) && (port <= GPIO3));
}

bool gpioCheckValidpin_number(uint8_t pin){
   return ((pin >= 0) && (pin <= 31));
}

bool gpioCheckValidDirection(pin_direction direction){
	return (direction == INPUT) || (direction ==  OUTPUT);
}

uint32_t gpioGetPort(gpio_port port) {
	uint32_t gpio_base;
	switch (port) {
		case GPIO0:
			gpio_base = SOC_GPIO_0_REGS;
			break;
		case GPIO1:
			gpio_base = SOC_GPIO_1_REGS;
			break;
		case GPIO2:
			gpio_base = SOC_GPIO_2_REGS;
			break;
		case GPIO3:
			gpio_base = SOC_GPIO_3_REGS;
			break;
		default:
			break;
	}
	return gpio_base;
}

// =============================================================================
// GPIO INTERRUPT
// =============================================================================

void gpioAintcConfigure(uint32_t int_line, uint32_t priority, void (*fnHandler)(void)) {
 
    /* Registering ISR for GPIO (Attach ISR to IRQ) */
    IntRegister(int_line, fnHandler);
    
    /* Set the priority */
    IntPrioritySet(int_line, priority, AINTC_HOSTINT_ROUTE_IRQ);
    
    /* Enable the system interrupt by removing the mask */
    IntSystemEnable(int_line);
   
}

void gpioPinIntEnable(gpio_handle_t *pin, uint32_t int_line) {
	uint32_t base_add = gpioGetPort(pin->port);
	if(int_line == GPIO_INTC_LINE_1) {
		HWREG(base_add + GPIO_IRQSTATUS_SET(0)) |= (1 << pin->pin_number);
	}
	else {
		HWREG(base_add + GPIO_IRQSTATUS_SET(1)) |= (1 << pin->pin_number);
	}
}

void gpioIntTypeSet(gpio_handle_t *pin, uint32_t event_type) {
    event_type &= 0xFF;
	uint32_t base_add = gpioGetPort(pin->port);
    switch(event_type) {

        case GPIO_INTC_TYPE_NO_LEVEL:

            /* Disabling logic LOW level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(0)) &= ~(1 << pin->pin_number);

            /* Disabling logic HIGH level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(1)) &= ~(1 << pin->pin_number);

        break;

        case GPIO_INTC_TYPE_LEVEL_LOW:

            /* Enabling logic LOW level detect interrupt geenration. */
            HWREG(base_add + GPIO_LEVELDETECT(0)) |= (1 << pin->pin_number);

            /* Disabling logic HIGH level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(1)) &= ~(1 << pin->pin_number);

            /* Disabling rising edge detect interrupt generation. */
            HWREG(base_add + GPIO_RISINGDETECT) &= ~(1 << pin->pin_number);

            /* Disabling falling edge detect interrupt generation. */
            HWREG(base_add + GPIO_FALLINGDETECT) &= ~(1 << pin->pin_number);

        break;

        case GPIO_INTC_TYPE_LEVEL_HIGH:

            /* Disabling logic LOW level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(0)) &= ~(1 << pin->pin_number);

            /* Enabling logic HIGH level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(1)) |= (1 << pin->pin_number);

            /* Disabling rising edge detect interrupt generation. */
            HWREG(base_add + GPIO_RISINGDETECT) &= ~(1 << pin->pin_number);

            /* Disabling falling edge detect interrupt generation. */
            HWREG(base_add + GPIO_FALLINGDETECT) &= ~(1 << pin->pin_number);
        
        break;

        case GPIO_INTC_TYPE_BOTH_LEVEL:
            
            /* Enabling logic LOW level detect interrupt geenration. */
            HWREG(base_add + GPIO_LEVELDETECT(0)) |= (1 << pin->pin_number);

            /* Enabling logic HIGH level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(1)) |= (1 << pin->pin_number);

            /* Disabling rising edge detect interrupt generation. */
            HWREG(base_add + GPIO_RISINGDETECT) &= ~(1 << pin->pin_number);

            /* Disabling falling edge detect interrupt generation. */
            HWREG(base_add + GPIO_FALLINGDETECT) &= ~(1 << pin->pin_number);
            
        break;

        case GPIO_INTC_TYPE_NO_EDGE:
            
            /* Disabling rising edge detect interrupt generation. */
            HWREG(base_add + GPIO_RISINGDETECT) &= ~(1 << pin->pin_number);

            /* Disabling falling edge detect interrupt generation. */
            HWREG(base_add + GPIO_FALLINGDETECT) &= ~(1 << pin->pin_number);

        break;

        case GPIO_INTC_TYPE_RISE_EDGE:

            /* Enabling rising edge detect interrupt generation. */
            HWREG(base_add + GPIO_RISINGDETECT) |= (1 << pin->pin_number);

            /* Disabling falling edge detect interrupt generation. */
            HWREG(base_add + GPIO_FALLINGDETECT) &= ~(1 << pin->pin_number);

            /* Disabling logic LOW level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(0)) &= ~(1 << pin->pin_number);

            /* Disabling logic HIGH level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(1)) &= ~(1 << pin->pin_number);

        break;

        case GPIO_INTC_TYPE_FALL_EDGE:

            /* Disabling rising edge detect interrupt generation. */
            HWREG(base_add + GPIO_RISINGDETECT) &= ~(1 << pin->pin_number);

            /* Enabling falling edge detect interrupt generation. */
            HWREG(base_add + GPIO_FALLINGDETECT) |= (1 << pin->pin_number);

            /* Disabling logic LOW level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(0)) &= ~(1 << pin->pin_number);

            /* Disabling logic HIGH level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(1)) &= ~(1 << pin->pin_number);

        break;

        case GPIO_INTC_TYPE_BOTH_EDGE:

            /* Enabling rising edge detect interrupt generation. */
            HWREG(base_add + GPIO_RISINGDETECT) |= (1 << pin->pin_number);

            /* Enabling falling edge detect interrupt generation. */
            HWREG(base_add + GPIO_FALLINGDETECT) |= (1 << pin->pin_number);

            /* Disabling logic LOW level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(0)) &= ~(1 << pin->pin_number);

            /* Disabling logic HIGH level detect interrupt generation. */
            HWREG(base_add + GPIO_LEVELDETECT(1)) &= ~(1 << pin->pin_number);

        break;

        default:
        break;
    }
}

bool gpioCheckIntFlag(gpio_handle_t *pin, uint32_t int_line) {
    uint32_t base_add = gpioGetPort(pin->port);
    bool flag;

	if(int_line == GPIO_INTC_LINE_1) {
		flag = (HWREG(base_add + GPIO_IRQSTATUS_0) >> pin->pin_number) & 1U;
	}
	else {
		flag = (HWREG(base_add + GPIO_IRQSTATUS_1) >> pin->pin_number) & 1U;
	}
    return flag;
}

void gpioClearIntFlag(gpio_handle_t *pin, uint32_t int_line) {
    uint32_t base_add = gpioGetPort(pin->port);

	if(int_line == GPIO_INTC_LINE_1) {
		HWREG(base_add + GPIO_IRQSTATUS_0) |= (1U << pin->pin_number);
	}
	else {
		HWREG(base_add + GPIO_IRQSTATUS_1) |= (1U << pin->pin_number);
	}
}