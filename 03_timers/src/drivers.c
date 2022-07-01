#include "drivers.h"
#include "system.h"
#include "timers.h"

#include <stdint.h>

#include "bbb_regs.h"
#include "hw_types.h"

static uint8_t cursor_position;

static uint32_t blink_time;

// =============================================================================
// PUBLIC FUNCTIONS IMPLEMENTATIONS
// =============================================================================

void buttonConfig(void) {
	/* Configure GPIO1 interrupt groups: Pin 12 to group A and pin 1 to group B. */
	HWREG(GPIO1_IRQSTATUS_SET_0) |= BUTTON_01_MASK; 
	HWREG(GPIO1_IRQSTATUS_SET_1) |= BUTTON_02_MASK;

  	/* Enable interrupt generation on detection of a rising edge.*/
	HWREG(GPIO1_RISINGDETECT)  |= BUTTON_01_MASK;
	HWREG(GPIO1_FALLINGDETECT) |= BUTTON_02_MASK;

    	/*  Configure Pin MUX for input GPIO without internal pull-up */
    	HWREG(CM_PER_GPMCAD12_REGS) |= (1U << 5) | (1U << 3) | (0b111U << 0);
    	HWREG(CM_PER_GPMCAD14_REGS) |= (1U << 5) | (1U << 3) | (0b111U << 0);
	
    	/* Configure the Buttons Pins for Input */
    	HWREG(GPIO1_OE) |= BUTTON_01_MASK | BUTTON_02_MASK; 
	
	cursor_position = 0;

}

void ledConfig(void) {
	/*  configure pin mux for output GPIO */
	HWREG(CM_PER_GPMCA5_REGS) |= (0b111U << 0);
	HWREG(CM_PER_GPMCA6_REGS) |= (0b111U << 0);
	HWREG(CM_PER_GPMCA7_REGS) |= (0b111U << 0);
	HWREG(CM_PER_GPMCA8_REGS) |= (0b111U << 0);

	HWREG(CM_PER_GPMCCSn0_REGS) |= (0b111U << 0);
	HWREG(CM_PER_GPMCAD13_REGS) |= (0b111U << 0);
	HWREG(CM_PER_GPMCA0_REGS)   |= (0b111U << 0);
	HWREG(CM_PER_GPMCA1_REGS)   |= (0b111U << 0);

	/* Configure the Led Pins for Output */
	HWREG(GPIO1_OE) &= ~LED_01_MASK & ~LED_02_MASK & ~LED_03_MASK & ~LED_04_MASK & ~LED_05_MASK & ~LED_06_MASK & ~LED_07_MASK & ~LED_08_MASK;

	ledOff(LED_01);
	ledOff(LED_02);
	ledOff(LED_03);
	ledOff(LED_04);
	ledOff(LED_05);
	ledOff(LED_06);
	ledOff(LED_07);
	ledOff(LED_08);

	blink_time = 250;
}

bool readButton(button_id btn) {
	bool value;
     switch (btn) {

		// PULL-UP
          case BUTTON_01:
               value = !((HWREG(BUTTON_01_DATAIN) >> BUTTON_01_BIT) & 1);
               break;

		// PULL-DOWN
          case BUTTON_02:
               value = (HWREG(BUTTON_02_DATAIN) >> BUTTON_02_BIT) & 1;
               break;
     }
	
	return value;
}

void ledOff(led_id led) {
	switch (led) {
		case LED_01:
			HWREG(LED_01_DATAOUT) &= ~LED_01_MASK;
			break;
		case LED_02:
			HWREG(LED_02_DATAOUT) &= ~LED_02_MASK;
			break;
		case LED_03:
			HWREG(LED_03_DATAOUT) &= ~LED_03_MASK;
			break;
		case LED_04:
			HWREG(LED_04_DATAOUT) &= ~LED_04_MASK;
			break;
		case LED_05:
			HWREG(LED_05_DATAOUT) &= ~LED_05_MASK;
			break;
		case LED_06:
			HWREG(LED_06_DATAOUT) &= ~LED_06_MASK;
			break;
		case LED_07:
			HWREG(LED_07_DATAOUT) &= ~LED_07_MASK;
			break;
		case LED_08:
			HWREG(LED_08_DATAOUT) &= ~LED_08_MASK;
			break;
		default:
			break;
	}
}

void ledOn(led_id led) {
	switch (led) {
		case LED_01:
			HWREG(LED_01_DATAOUT) |= LED_01_MASK;
			break;
		case LED_02:
			HWREG(LED_02_DATAOUT) |= LED_02_MASK;
			break;
		case LED_03:
			HWREG(LED_03_DATAOUT) |= LED_03_MASK;
			break;
		case LED_04:
			HWREG(LED_04_DATAOUT) |= LED_04_MASK;
			break;
		case LED_05:
			HWREG(LED_05_DATAOUT) |= LED_05_MASK;
			break;
		case LED_06:
			HWREG(LED_06_DATAOUT) |= LED_06_MASK;
			break;
		case LED_07:
			HWREG(LED_07_DATAOUT) |= LED_07_MASK;
			break;
		case LED_08:
			HWREG(LED_08_DATAOUT) |= LED_08_MASK;
			break;
		default:
			break;
	}
}

void toggleLeds(void) {
	HWREG(LED_01_DATAOUT) ^= LED_01_MASK;
	HWREG(LED_02_DATAOUT) ^= LED_02_MASK;
	HWREG(LED_03_DATAOUT) ^= LED_03_MASK;
	HWREG(LED_04_DATAOUT) ^= LED_04_MASK;
	HWREG(LED_05_DATAOUT) ^= LED_05_MASK;
	HWREG(LED_06_DATAOUT) ^= LED_06_MASK;
	HWREG(LED_07_DATAOUT) ^= LED_07_MASK;
	HWREG(LED_08_DATAOUT) ^= LED_08_MASK;
}

uint32_t getBlinkTime(void) {
	return blink_time;
}

void scrollCursorDown(void) {
	// Cursor go down one position. If is the last position goes back to top.
	cursor_position = (cursor_position + 1) % NUMBER_CURSOR_POSITIONS;
}

void selectCurrentOption(void) {
	switch(cursor_position) {
		case 0:
			blink_time = 250;
			break;
		case 1:
			blink_time = 500;
			break;
		case 2:
			blink_time = 1000;
			break;
		case 3:
			blink_time = 2000;
			break;
		default:
			break;
	}

	// Restart the timer with the selected time.
	timerStop();
	timerStart(blink_time);
}

uint8_t getCursorPosition() {
     return cursor_position;
}