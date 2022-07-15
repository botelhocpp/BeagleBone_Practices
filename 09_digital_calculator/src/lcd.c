#include "lcd.h"

#include "gpio.h"
#include "timers.h"

// ==========================================================================
// LCD INSTRUCTION CODES
// ==========================================================================

#define DISPLAY_TURN_ON  					0x0C
#define DISPLAY_TURN_OFF 					0x08
#define DISPLAY_CLEAR				 		0x01

#define CURSOR_TURN_ON				 		0x0E
#define CURSOR_TURN_OFF				 		0x0C
#define CURSOR_SHIFT_LEFT				 	0x10
#define CURSOR_SHIFT_RIGHT				 	0x14
#define CURSOR_RETURN				 		0x02
#define CURSOR_BLINK				 		0x0D
#define CURSOR_ALT				 			0x0F

#define WRITE_SHIFT_CURSOR_LEFT 				0x04
#define WRITE_SHIFT_CURSOR_RIGHT 				0x06

#define SHIFT_MESSAGE_LEFT 					0x18
#define SHIFT_MESSAGE_RIGHT 					0x1C

#define DISPLAY_2L_5X7_4BIT					0x28
#define DISPLAY_2L_5X7_8BIT					0x38

#define CURSOR_START_FIRST_LINE				0x80
#define CURSOR_START_SECOND_LINE				0xC0

#define MEMORY_SET_CGRAM						0x40

// ==========================================================================
// PRIVATE VARIABLES
// ==========================================================================
const uint8_t big_numbers_codes[] = {

								 0b00000001,//0
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00011111,
								 0b00000000,

								 0b00011111,//1
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00000000,

								 0b00011111,//2
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000000,

								 0b00000001,//3
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000000,

								 0b00011111,//4
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00011111,
								 0b00000000,

								 0b00011111,//5
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00011111,
								 0b00000000,

								 0b00011111,//6
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,

								 0b00011111,//7
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00011111,
								 0b00000000
};

static const uint8_t big_numbers_commands[10][4] =
{
		{0x01, 0x02, 0x4C, 0x00}, //nr. 0
		{0x20, 0x7C, 0x20, 0x7C}, //nr. 1
		{0x04, 0x05, 0x4C, 0x5F}, //nr. 2
		{0x06, 0x05, 0x5F, 0x00}, //nr. 3
		{0x4C, 0x00, 0x20, 0x03}, //nr. 4
		{0x07, 0x04, 0x5F, 0x00}, //nr. 5
		{0x07, 0x04, 0x4C, 0x00}, //nr. 5
		{0x06, 0x02, 0x20, 0x03}, //nr. 7
		{0x07, 0x05, 0x4C, 0x00}, //nr. 8
		{0x07, 0x05, 0x20, 0x03}  //nr. 9
};

// ==========================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// ==========================================================================

void lcdSendCommand(lcd_handler_t *lcd, uint8_t command);

void lcdSendData(lcd_handler_t *lcd, uint8_t data);

void lcdSendBits(lcd_handler_t *lcd, uint8_t byte);

void lcdEnablePulse(lcd_handler_t *lcd);

// ==========================================================================
// PUBLIC FUNCTIONS IMPLEMENTATION
// ==========================================================================

void lcdInitPinsModule(lcd_handler_t *lcd) {
	gpioPInitPin(&lcd->data[0], OUTPUT);
	gpioPInitPin(&lcd->data[1], OUTPUT);
	gpioPInitPin(&lcd->data[2], OUTPUT);
	gpioPInitPin(&lcd->data[3], OUTPUT);
	gpioPInitPin(&lcd->rs, OUTPUT);
	gpioPInitPin(&lcd->en, OUTPUT);
	
	lcdInitModule(lcd);
}

void lcdInitModule(lcd_handler_t *lcd) {
	// WAIT 30ms FOR LCD TO STABILIZE THE VOLTAGE
	delay_ms(30);

	lcdSendCommand(lcd, CURSOR_SHIFT_RIGHT);
	lcdSendCommand(lcd, CURSOR_RETURN);
	lcdSendCommand(lcd, DISPLAY_2L_5X7_4BIT);
	lcdSendCommand(lcd, DISPLAY_TURN_OFF);
	lcdSendCommand(lcd, DISPLAY_CLEAR);
	lcdSendCommand(lcd, CURSOR_TURN_OFF);
	lcdSendCommand(lcd, CURSOR_START_FIRST_LINE);
	// lcdSendCommand(lcd, CURSOR_BLINK);
}

void lcdClearDisplay(lcd_handler_t *lcd) {
	lcdSendCommand(lcd, DISPLAY_CLEAR);
	lcdSendCommand(lcd, CURSOR_RETURN);
}

void lcdSetCursor(lcd_handler_t *lcd, uint8_t row, uint8_t columm) {
	if(row > 1 || row < 0) {
		while(1);
	}
	uint8_t cursor_add = (row == 0) ? CURSOR_START_FIRST_LINE : CURSOR_START_SECOND_LINE;
	lcdSendCommand(lcd, cursor_add + columm);
}

void lcdWriteString(lcd_handler_t *lcd, char *lcd_str) {
	for(int i = 0; lcd_str[i] != '\0'; i++) {
		lcdSendData(lcd, (unsigned int) lcd_str[i]);
	}
}

void lcdWriteChar(lcd_handler_t *lcd, char lcd_char) {
	lcdSendData(lcd, (unsigned int) lcd_char);
}

void lcdReturnCursor(lcd_handler_t *lcd) {
	lcdSendCommand(lcd, CURSOR_RETURN);
}

void lcdScrollLeft(lcd_handler_t *lcd) {
	lcdSendCommand(lcd, SHIFT_MESSAGE_LEFT);
}

void lcdScrollRight(lcd_handler_t *lcd) {
	lcdSendCommand(lcd, SHIFT_MESSAGE_RIGHT);
}


void lcdCreateChar(lcd_handler_t *lcd, lcd_custom_char custom_char, char *char_map) {
	if(custom_char > lcdCUSTOM_CHAR_8 || custom_char < lcdCUSTOM_CHAR_1) {
		while(1);
	}

	lcdSendCommand(lcd, MEMORY_SET_CGRAM | (custom_char << 3));

	for (int i = 0; i<8; i++) {
		lcdSendData(lcd, char_map[i]);
	}
}

void lcdCreateBigNumbers(lcd_handler_t *lcd) {

	for (int i = 0, j = 0; i < 8; i++) {
		lcdCreateChar(lcd, i, &big_numbers_codes[j]);
		j += 8;
	}
}

void lcdWriteBigNumber(lcd_handler_t *lcd, uint8_t columm, uint8_t number) {

	lcdSetCursor(lcd, columm, 0);
	lcdSendData(lcd, big_numbers_commands[number][0]);
	lcdSendData(lcd, big_numbers_commands[number][1]);

	lcdSetCursor(lcd, columm, 1);
	lcdSendData(lcd, big_numbers_commands[number][2]);
	lcdSendData(lcd, big_numbers_commands[number][3]);
}

// ==========================================================================
// PRIVATE FUNCTIONS IMPLEMENTATIONS
// ==========================================================================

void lcdSendCommand(lcd_handler_t *lcd, uint8_t command) {
	// LOGIC 0 IN RS FOR COMMAND
	gpioSetPinValue(&lcd->rs, LOW);
	
	// SEND THE COMMAND VIA 4-BIT INTERFACE
	#ifdef LCD_MODE_4_BITS
	lcdSendBits(lcd, command >> 4);
	#endif
	lcdSendBits(lcd, command);

	// IF RETURN COMMAND OR CLEAR WAIT FOR LCD
	if (command < 4) {
		delay_ms(4);
	}
	else {
		delay_us(100);
	}
}

void lcdSendData(lcd_handler_t *lcd, uint8_t data) {
	// LOGIC 1 IN RS FOR DATA
	gpioSetPinValue(&lcd->rs, HIGH);

	// SEND THE DATA VIA 4-BIT INTERFACE
	lcdSendBits(lcd, data >> 4);
	lcdSendBits(lcd, data);
}

void lcdSendBits(lcd_handler_t *lcd, uint8_t byte) {
	for(int i = 0; i < 4; i++) {
		gpioSetPinValue(&lcd->data[i], ((byte >> i) & 1U));
	}
	lcdEnablePulse(lcd);
}

void lcdEnablePulse(lcd_handler_t *lcd) {
	// LOGIC 0 IN EN
	gpioSetPinValue(&lcd->en, LOW);

	delay_us(1);

	// LOGIC 1 IN EN
	gpioSetPinValue(&lcd->en, HIGH);

	// WAIT 1us
	delay_us(1);

	// LOGIC 0 IN EN
	gpioSetPinValue(&lcd->en, LOW);

	// WAIT 45us
	delay_us(100);
}
