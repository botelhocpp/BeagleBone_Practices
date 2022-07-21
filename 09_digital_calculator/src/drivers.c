#include "drivers.h"
#include "gpio.h"
#include "lcd.h"
#include "interrupt.h"
#include "timers.h"

#include "uart.h"

#include <stdint.h>

// =============================================================================
// GPIO PINS
// =============================================================================

static lcd_handler_t lcd;

static gpio_handle_t clear_btn;
static gpio_handle_t led01;
static gpio_handle_t led02;
static gpio_handle_t led03;
static gpio_handle_t led04;

static gpio_handle_t l1;
static gpio_handle_t l2;
static gpio_handle_t l3;
static gpio_handle_t l4;

static gpio_handle_t c1;
static gpio_handle_t c2;
static gpio_handle_t c3;
static gpio_handle_t c4;

// =============================================================================
// PRIVATE VARIABLES
// =============================================================================

/**
 * @brief Keeps the last pressed key of the keyboard.
 */
static char last_pressed_key = 0;

/**
 * @brief Keeps the keyboard read status.
 */
static bool read_flag = true;

// =============================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// =============================================================================

void sysModulesInit(void);

void drvLedInit(void);

void drvLcdInit(void);

void drvButtonInit(void);

void drvKeyboardInit(void);

uint8_t drvKeyboardVerifyKey(gpio_handle_t *column);

// =============================================================================
// PRIVATE ISR PROTOTYPES
// =============================================================================

void irqHandlerGpio1A(void);

void irqHandlerGpio1B(void);

// =============================================================================
// PUBLIC FUNCTIONS
// =============================================================================

void drvComponentInit(void) {
     /* Enable the GPIO modules, disable WD and Enable the INTC */
     sysModulesInit();
     drvButtonInit();
     drvKeyboardInit();
     drvLcdInit();
}

char drvReadPressedKey() {
     /* Keyboard is being read */
     read_flag = true;

     /* Get the last pressed key */
     return last_pressed_key;
}

bool drvCheckReadFlag() {
     return read_flag;
}

// =============================================================================
// PRIVATE ISR IMPLEMENTATION
// =============================================================================

void irqHandlerGpio1A(void) {
     /* Signals that the last key value was modified */
     read_flag = false;

     /* Checks if the Button pin caused the Interrupt */
	if(gpioCheckIntFlag(&clear_btn, GPIO_INTC_LINE_1)) {
          /* Update the Current Function to 'Clear' */
          last_pressed_key = 'C';

          /* Clears the Interrupt Pending Flag */
          gpioClearIntFlag(&clear_btn, GPIO_INTC_LINE_1);
	}

     /* Software Debouncing */
     delay_ms(60);
}

void irqHandlerGpio1B(void) {
     /* Signals that the new key was not read */ 
     read_flag = false;

     /* Checks if the column 01 pin cause the interrupt in the GPIO1B group */
	if(gpioCheckIntFlag(&c1, GPIO_INTC_LINE_2)) {
          /* Obtains the line in which the pressed button is located */
          uint8_t line = drvKeyboardVerifyKey(&c1);

          /* Update the last pressed key based in the line and column */
          switch(line) {
               case 1:
                    last_pressed_key = '1';
                    break;
               case 2:
                    last_pressed_key = '4';
                    break;
               case 3:
                    last_pressed_key = '7';
                    break;
               case 4:
                    last_pressed_key = '*';
                    break;
               default:
                    break;
          }
          
          /* Clear interrupt pending flag*/
          gpioClearIntFlag(&c1, GPIO_INTC_LINE_2);
     }
	else if(gpioCheckIntFlag(&c2, GPIO_INTC_LINE_2)) {
          uint8_t line = drvKeyboardVerifyKey(&c2);
          switch(line) {
               case 1:
                    last_pressed_key = '2';
                    break;
               case 2:
                    last_pressed_key = '5';
                    break;
               case 3:
                    last_pressed_key = '8';
                    break;
               case 4:
                    last_pressed_key = '0';
                    break;
               default:
                    break;
          }
          gpioClearIntFlag(&c2, GPIO_INTC_LINE_2);
     }
	else if(gpioCheckIntFlag(&c3, GPIO_INTC_LINE_2)) {
          uint8_t line = drvKeyboardVerifyKey(&c3);
          switch(line) {
               case 1:
                    last_pressed_key = '3';
                    break;
               case 2:
                    last_pressed_key = '6';
                    break;
               case 3:
                    last_pressed_key = '9';
                    break;
               case 4:
                    last_pressed_key = '=';
                    break;
               default:
                    break;
          }
          gpioClearIntFlag(&c3, GPIO_INTC_LINE_2);
     }
	else if(gpioCheckIntFlag(&c4, GPIO_INTC_LINE_2)) {
          uint8_t line = drvKeyboardVerifyKey(&c4);
          switch(line) {
               case 1:
                    last_pressed_key = 'D';
                    break;
               case 2:
                    last_pressed_key = '+';
                    break;
               case 3:
                    last_pressed_key = '-';
                    break;
               case 4:
                    last_pressed_key = '/';
                    break;
               default:
                    break;
          }
          gpioClearIntFlag(&c4, GPIO_INTC_LINE_2);
	}

     /* Software Debouncing */
     delay_ms(30);
}

// =============================================================================
// PRIVATE FUNCTIONS IMPLEMENTATION
// =============================================================================

void sysModulesInit(void) {
     /* Disable WD to avoid System Reset */
     IntDisableWatchdog();

     /* Initialize the Interrupt Controller (INTC) */
     IntAINTCInit();

     /* Enables the GPIO Modules by enabling their clock */
     gpioInitModule(GPIO0);
     gpioInitModule(GPIO1);
     gpioInitModule(GPIO2);
     gpioInitModule(GPIO3);

     timerInitModule();
}

void drvLedInit(void) {
     
     led01.port = GPIO1;
     led01.pin_number = 24;
     gpioPInitPin(&led01, OUTPUT);

     led02.port = GPIO1;
     led02.pin_number = 23;
     gpioPInitPin(&led02, OUTPUT);

     led03.port = GPIO1;
     led03.pin_number = 22;
     gpioPInitPin(&led03, OUTPUT);

     led04.port = GPIO1;
     led04.pin_number = 21;
     gpioPInitPin(&led04, OUTPUT);
}

void drvLcdInit(void) {
     gpio_handle_t rs;
     rs.port = GPIO1;
     rs.pin_number = 16;
     gpioPInitPin(&rs, OUTPUT);

     gpio_handle_t en;
     en.port = GPIO0;
     en.pin_number = 7;
     gpioPInitPin(&en, OUTPUT);

     gpio_handle_t d4;
     d4.port = GPIO1;
     d4.pin_number = 17;
     gpioPInitPin(&d4, OUTPUT);

     gpio_handle_t d5;
     d5.port = GPIO3;
     d5.pin_number = 21;
     gpioPInitPin(&d5, OUTPUT);

     gpio_handle_t d6;
     d6.port = GPIO3;
     d6.pin_number = 19;
     gpioPInitPin(&d6, OUTPUT);

     gpio_handle_t d7;
     d7.port = GPIO1;
     d7.pin_number = 28;
     gpioPInitPin(&d7, OUTPUT);

     lcd.rs = rs;
     lcd.en = en;
     lcd.data[0] = d4;
     lcd.data[1] = d5;
     lcd.data[2] = d6;
     lcd.data[3] = d7;

     lcdInitModule(&lcd);
}

void drvButtonInit(void) {
     /* Initialize the Red Button (GPIO1_9) as Input */
     clear_btn.port = GPIO1;
     clear_btn.pin_number = 2;
     gpioPInitPin(&clear_btn, INPUT);

     /* Configure the GPIO1A Interrupt Group as Maximum Priority and attach an ISR */
     gpioAintcConfigure(SYS_INT_GPIOINT1A, 0, irqHandlerGpio1A);

     /* Enable Interrupts for the Button Pin in GPIO1A */
     gpioPinIntEnable(&clear_btn, GPIO_INTC_LINE_1);

     /* Configures the Interrupt Trigger */
     gpioIntTypeSet(&clear_btn, GPIO_INTC_TYPE_RISE_EDGE);
}

void drvKeyboardInit(void) {
     
     l1.port = GPIO0;
     l1.pin_number = 26;
     gpioPInitPin(&l1, OUTPUT);
     gpioSetPinValue(&l1, HIGH);

     l2.port = GPIO2;
     l2.pin_number = 1;
     gpioPInitPin(&l2, OUTPUT);
     gpioSetPinValue(&l2, HIGH);

     l3.port = GPIO0;
     l3.pin_number = 27;
     gpioPInitPin(&l3, OUTPUT);
     gpioSetPinValue(&l3, HIGH);

     l4.port = GPIO1;
     l4.pin_number = 29;
     gpioPInitPin(&l4, OUTPUT);
     gpioSetPinValue(&l4, HIGH);

     /* Enable GPIO1B Interrupts with Maximum Priority and Attach ISR */
     gpioAintcConfigure(SYS_INT_GPIOINT1B, 0, irqHandlerGpio1B); 

     c1.port = GPIO1;
     c1.pin_number = 12; 
     gpioPInitPin(&c1, INPUT);
     gpioConfigPull(&c1, PULLDOWN);
     gpioPinIntEnable(&c1, GPIO_INTC_LINE_2);
     gpioIntTypeSet(&c1, GPIO_INTC_TYPE_RISE_EDGE);

     c2.port = GPIO1;
     c2.pin_number = 14;
     gpioPInitPin(&c2, INPUT);
     gpioConfigPull(&c2, PULLDOWN);
     gpioPinIntEnable(&c2, GPIO_INTC_LINE_2);
     gpioIntTypeSet(&c2, GPIO_INTC_TYPE_RISE_EDGE);

     c3.port = GPIO1;
     c3.pin_number = 13;
     gpioPInitPin(&c3, INPUT);
     gpioConfigPull(&c3, PULLDOWN);
     gpioPinIntEnable(&c3, GPIO_INTC_LINE_2);
     gpioIntTypeSet(&c3, GPIO_INTC_TYPE_RISE_EDGE);

     c4.port = GPIO1;
     c4.pin_number = 15;
     gpioPInitPin(&c4, INPUT);
     gpioConfigPull(&c4, PULLDOWN);
     gpioPinIntEnable(&c4, GPIO_INTC_LINE_2);
     gpioIntTypeSet(&c4, GPIO_INTC_TYPE_RISE_EDGE);
}

uint8_t drvKeyboardVerifyKey(gpio_handle_t *column) {
     uint8_t line = 0;

     gpioSetPinValue(&l1, LOW); 
     gpioSetPinValue(&l2, LOW); 
     gpioSetPinValue(&l3, LOW); 
     gpioSetPinValue(&l4, LOW); 

     gpioSetPinValue(&l1, HIGH);
     if(gpioGetPinValue(column)) {
          line = 1;
          goto ret;
     }    
     gpioSetPinValue(&l1, LOW);

     gpioSetPinValue(&l2, HIGH); 
     if(gpioGetPinValue(column)) {
          line = 2;
          goto ret;
     }    
     gpioSetPinValue(&l2, LOW); 

     gpioSetPinValue(&l3, HIGH); 
     if(gpioGetPinValue(column)) {
          line = 3;
          goto ret;
     } 
     gpioSetPinValue(&l3, LOW);   

     gpioSetPinValue(&l4, HIGH); 
     if(gpioGetPinValue(column)) {
          line = 4;
          goto ret;
     }   
     gpioSetPinValue(&l4, LOW); 

     ret:
     gpioSetPinValue(&l1, HIGH); 
     gpioSetPinValue(&l2, HIGH); 
     gpioSetPinValue(&l3, HIGH); 
     gpioSetPinValue(&l4, HIGH); 

     return line;
}

lcd_handler_t *drvGetLcdHandler(void) {
     return &lcd;
}
