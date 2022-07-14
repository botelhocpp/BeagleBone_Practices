#ifndef LCD_H
#define LCD_H

#include <gpio.h>

#define LCD_MODE_4_BITS

typedef enum {
	lcdCUSTOM_CHAR_1,
	lcdCUSTOM_CHAR_2,
	lcdCUSTOM_CHAR_3,
	lcdCUSTOM_CHAR_4,
	lcdCUSTOM_CHAR_5,
	lcdCUSTOM_CHAR_6,
	lcdCUSTOM_CHAR_7,
	lcdCUSTOM_CHAR_8
} lcd_custom_char;

typedef struct {
	gpio_handle_t data[4];
	gpio_handle_t rs;
	gpio_handle_t en;
} lcd_handler_t;

extern void lcdInitModule(lcd_handler_t *lcd);

extern void lcdInitPinsModule(lcd_handler_t *lcd);

extern void lcdClearDisplay(lcd_handler_t *lcd);

extern void lcdSetCursor(lcd_handler_t *lcd, uint8_t row, uint8_t columm);

extern void lcdWriteString(lcd_handler_t *lcd, char *lcd_str);

extern void lcdWriteChar(lcd_handler_t *lcd, char lcd_char);

extern void lcdReturnCursor(lcd_handler_t *lcd);

extern void lcdScrollLeft(lcd_handler_t *lcd);

extern void lcdScrollRight(lcd_handler_t *lcd);

extern void lcdCreateChar(lcd_handler_t *lcd, lcd_custom_char custom_char, char *char_map);

extern void lcdCreateBigNumbers(lcd_handler_t *lcd);

extern void lcdWriteBigNumber(lcd_handler_t *lcd, uint8_t columm, uint8_t number);

#endif
