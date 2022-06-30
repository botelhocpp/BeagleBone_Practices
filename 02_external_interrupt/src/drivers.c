#include "drivers.h"
#include "system.h"

#include "bbb_regs.h"
#include "hw_types.h"

static int blink_mode;

void buttonConfig(void) {
	/* Configure GPIO1 interrupt groups. */
	HWREG(GPIO1_IRQSTATUS_SET_0) |= BUTTON_01_MASK; 
	HWREG(GPIO1_IRQSTATUS_SET_1) |= BUTTON_02_MASK;

	/* Configure GPIO2 interrupt group. */
	HWREG(GPIO2_IRQSTATUS_SET_0) |= BUTTON_03_MASK; 

  	/* Enable interrupt generation on detection of a rising edge.*/
	HWREG(GPIO1_RISINGDETECT) |= BUTTON_01_MASK | BUTTON_02_MASK;
	HWREG(GPIO2_RISINGDETECT) |= BUTTON_03_MASK;

    	/*  Configure Pin MUX for input GPIO without internal pull-up */
    	HWREG(CM_PER_GPMCAD12_REGS) |= (1U << 5) | (1U << 3) | (0b111U << 0);
    	HWREG(CM_PER_GPMCAD14_REGS) |= (1U << 5) | (1U << 3) | (0b111U << 0);
    	HWREG(CM_PER_GPMCCLK_REGS)  |= (1U << 5) | (1U << 3) | (0b111U << 0);
	
    	/* Configure the Buttons Pins for Input */
    	HWREG(GPIO1_OE) |= BUTTON_01_MASK | BUTTON_02_MASK; 
    	HWREG(GPIO2_OE) |= BUTTON_03_MASK; 
	
	blink_mode = 0;

}

void ledConfig(void) {
    /*  configure pin mux for output GPIO */
    HWREG(CM_PER_GPMCA5_REGS) |= (0b111U << 0);
    HWREG(CM_PER_GPMCA6_REGS) |= (0b111U << 0);
    HWREG(CM_PER_GPMCA7_REGS) |= (0b111U << 0);
    HWREG(CM_PER_GPMCA8_REGS) |= (0b111U << 0);

    HWREG(GPIO1_OE) &= ~LED_01_MASK & ~LED_02_MASK & ~LED_03_MASK & ~LED_04_MASK;

    ledOff(LED_01);
    ledOff(LED_02);
    ledOff(LED_03);
    ledOff(LED_04);
}

bool readButton(button_id btn) {
	bool value;
     switch (btn) {
          case BUTTON_01:
               value = (HWREG(BUTTON_01_DATAIN) >> BUTTON_01_BIT) & 1;
               break;
          case BUTTON_02:
               value = (HWREG(BUTTON_02_DATAIN) >> BUTTON_02_BIT) & 1;
               break;
          case BUTTON_03:
               value = (HWREG(BUTTON_03_DATAIN) >> BUTTON_03_BIT) & 1;
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
		default:
			break;
	}
}

void ledSequence01(void) {
	ledOn(LED_01);
	ledOn(LED_02);
	delay_ms(500);

	ledOff(LED_01);
	ledOff(LED_02);
	ledOn(LED_03);
	ledOn(LED_04);
	delay_ms(500);

	ledOff(LED_03);
	ledOff(LED_04);
}

void ledSequence02(void) {
	ledOn(LED_01);
	delay_ms(500);
	ledOn(LED_02);
	delay_ms(500);
	ledOn(LED_03);
	delay_ms(500);
	ledOn(LED_04);
	delay_ms(500);

	ledOff(LED_01);
	delay_ms(500);
	ledOff(LED_02);
	delay_ms(500);
	ledOff(LED_03);
	delay_ms(500);
	ledOff(LED_04);
	delay_ms(500);
}

void ledSequence03(void) {
	for (int i = 0; i < 5; i++) {
		ledOn(LED_01);
		ledOn(LED_04);
		ledOff(LED_02);
		ledOff(LED_03);
		delay_ms(250);

		ledOff(LED_01);
		ledOff(LED_04);
		ledOn(LED_02);
		ledOn(LED_03);
		delay_ms(250);
	}
	ledOff(LED_02);
	ledOff(LED_03);
	delay_ms(250);
}

void ledSequence04(void) {
	for (int i = 0; i < 5; i++) {
		ledOn(LED_01);
		ledOn(LED_03);
		delay_ms(500);

		ledOn(LED_02);
		ledOn(LED_04);
		ledOff(LED_01);
		ledOff(LED_03);
		delay_ms(500);

		ledOff(LED_02);
		ledOff(LED_04);
	}
	delay_ms(500);
}

void buttonLedTest(void) {
	while(true) {
		if(readButton(BUTTON_01)) {
			ledOn(LED_01);
			//putString("LED 01 ON!\n\r", 13);
		}
		else {
			ledOff(LED_01);
		}

		if(readButton(BUTTON_02)) {
			ledOn(LED_02);
			//putString("LED 02 ON!\n\r", 13);
		}
		else {
			ledOff(LED_02);
		}

		if(readButton(BUTTON_03)) {
			ledOn(LED_03);
			//putString("LED 03 ON!\n\r", 13);
		}
		else {
			ledOff(LED_03);
		}
	}
}

void setBlinkMode(int mode) {
     blink_mode = (mode >= 0 && mode < 4) ? mode : 0;
}

int getBlinkMode() {
     return blink_mode;
}