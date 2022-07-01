/**
 * @file main.c
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @brief Controls the blinking time of eight LEDs using
 * two buttons: one (pull-down) on GPIO1_12, which will
 * select a time option, and another (pull-up) on GPIO1_14,
 * which will confirm this option.
 * @version 1.0
 * @date 2022-07-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "bbb_regs.h"
#include "hw_types.h"

#include "drivers.h"
#include "timers.h"
#include "system.h"
#include "uart.h"
#include "gpio.h"

void showMenu(uint8_t cursor_position);

int main(void){
	
	/* Hardware setup */
	systemInit();
	gpioSetup();
	ledConfig();
	buttonConfig();
	timerSetup();

	putString("Start!\n\r", 9);

	uint8_t previous_cursor_position = -1;
	uint8_t current_cursor_position = 0;

	timerStart(getBlinkTime());

	while(true){
		// If the cursor changes (the button in GPIO1_12 is pressed) show the menu.
		current_cursor_position = getCursorPosition();
		if(current_cursor_position != previous_cursor_position) {
			showMenu(current_cursor_position);
		}
		previous_cursor_position = current_cursor_position;
	}
}

void showMenu(uint8_t cursor_position) {
	putString("\033[H\033[J\r", 8);
	putString("\033[1;94m--------------------------------------------------------------------------------------\033[0m\r\n", 100);
	putString("  Select the flashing period with the \033[1;40mblack button\033[0m and confirm with the \033[1;41mred button\033[0m.\r\n", 108);
	putString("\033[1;94m--------------------------------------------------------------------------------------\033[0m\r\n\r\n", 102);
	switch (cursor_position) {
		case 0:
			putString("\033[1;42m1) 250 ms <-\033[0m\r\n2) 500 ms\r\n3) 1 s\r\n4) 2 s\r\n", 50);
			break;
		case 1:
			putString("1) 250 ms\r\n\033[1;42m2) 500 ms <-\033[0m\r\n3) 1 s\r\n4) 2 s\r\n", 50);
			break;
		case 2:
			putString("1) 250 ms\r\n2) 500 ms\r\n\033[1;42m3) 1 s <-\033[0m\r\n4) 2 s\r\n", 50);
			break;
		case 3:
			putString("1) 250 ms\r\n2) 500 ms\r\n3) 1 s\r\n\033[1;42m4) 2 s <-\033[0m\r\n", 50);
			break;
		default:
			break;
	}
}


