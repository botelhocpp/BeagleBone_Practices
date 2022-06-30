/**
 * @file main.c
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @brief Three buttons control the LED blink sequence. Each button
 * trigger a diferent blink sequence. After the sequence ends the
 * default sequence starts over.
 * Button 01: GPIO1_12, in pin GPMC_AD12.
 * Button 02: GPIO1_14, in pin GPMC_AD14 
 * Button 03: GPIO2_1, in pin GPMC_CLK
 * Use of the four embedded LEDs of BBB board.
 * @version 1.0
 * @date 2022-06-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "bbb_regs.h"
#include "hw_types.h"

#include "drivers.h"
#include "system.h"
#include "uart.h"
#include "gpio.h"

int main(void){
	
	/* Hardware setup */
	systemInit();
	gpioSetup();
	ledConfig();
	buttonConfig();

	putString("Start!\n\r", 9);
	delay_ms(500);
	
	while(true){
		switch(getBlinkMode()) {
			case 0:
				// Default Blink Sequence.
				ledSequence01();	
				break;
			case 1:
				putString("Button 01 pressed!\n\r", 21);
				ledSequence02();
				setBlinkMode(0);
				break;
			case 2:
				putString("Button 02 pressed!\n\r", 21);
				ledSequence03();
				setBlinkMode(0);
				break;
			case 3:
				putString("Button 03 pressed!\n\r", 21);
				ledSequence04();
				setBlinkMode(0);
				break;
		}
	}
}




