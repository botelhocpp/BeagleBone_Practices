#include "drivers.h"
#include "gpio.h"
#include "interrupt.h"

#include "uart.h"

#include <stdint.h>

// =============================================================================
// GPIO PINS
// =============================================================================

static gpio_handle_t btn;
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

static int8_t last_pressed_key = 0;

static bool read_flag = true;

// =============================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// =============================================================================

void sysModulesInit(void);

void drvLedInit(void);

void drvButtonInit(void);

void drvKeyboardInit(void);

void drvDelayMs(int ms);

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
     sysModulesInit();
     drvLedInit();
     drvButtonInit();
     drvKeyboardInit();
     putString("Config OK!\n\r", 13);
}

int8_t drvLastPressedKey() {
     return last_pressed_key;
}

bool drvGetReadFlag() {
     return read_flag;
}

void drvSetReadFlag() {
     read_flag = true;
}

// =============================================================================
// PRIVATE ISR IMPLEMENTATION
// =============================================================================

void irqHandlerGpio1A(void) {
     read_flag = false;
	if(gpioCheckIntFlag(&btn, GPIO_INTC_LINE_1)) {
          last_pressed_key = -1;
          //gpioTogglePinValue(&led01);
          //gpioTogglePinValue(&led02);
          //gpioTogglePinValue(&led03);
          //gpioTogglePinValue(&led04);
          gpioClearIntFlag(&btn, GPIO_INTC_LINE_1);
	}
}

void irqHandlerGpio1B(void) {
     read_flag = false;
	if(gpioCheckIntFlag(&c1, GPIO_INTC_LINE_2)) {
          uint8_t column = drvKeyboardVerifyKey(&c1);
          // last_pressed_key = column;

          switch(column) {
               case 1:
                    last_pressed_key = 1;
                    break;
               case 2:
                    last_pressed_key = 4;
                    break;
               case 3:
                    last_pressed_key = 7;
                    break;
               case 4:
                    last_pressed_key = 14;
                    break;
               default:
                    break;
          }
          gpioClearIntFlag(&c1, GPIO_INTC_LINE_2);
     }
	else if(gpioCheckIntFlag(&c2, GPIO_INTC_LINE_2)) {
          uint8_t column = drvKeyboardVerifyKey(&c2);
          //last_pressed_key = column + 4;

          switch(column) {
               case 1:
                    last_pressed_key = 2;
                    break;
               case 2:
                    last_pressed_key = 5;
                    break;
               case 3:
                    last_pressed_key = 8;
                    break;
               case 4:
                    last_pressed_key = 0;
                    break;
               default:
                    break;
          }
          gpioClearIntFlag(&c2, GPIO_INTC_LINE_2);
     }
	else if(gpioCheckIntFlag(&c3, GPIO_INTC_LINE_2)) {
          uint8_t column = drvKeyboardVerifyKey(&c3);
          //last_pressed_key = column + 8;

          switch(column) {
               case 1:
                    last_pressed_key = 3;
                    break;
               case 2:
                    last_pressed_key = 6;
                    break;
               case 3:
                    last_pressed_key = 9;
                    break;
               case 4:
                    last_pressed_key = 15;
                    break;
               default:
                    break;
          }
          gpioClearIntFlag(&c3, GPIO_INTC_LINE_2);
     }
	else if(gpioCheckIntFlag(&c4, GPIO_INTC_LINE_2)) {
          uint8_t column = drvKeyboardVerifyKey(&c4);
          //last_pressed_key = column + 12;

          switch(column) {
               case 1:
                    last_pressed_key = 10;
                    break;
               case 2:
                    last_pressed_key = 11;
                    break;
               case 3:
                    last_pressed_key = 12;
                    break;
               case 4:
                    last_pressed_key = 13;
                    break;
               default:
                    break;
          }
          gpioClearIntFlag(&c4, GPIO_INTC_LINE_2);
	}
     drvDelayMs(6);
}

// =============================================================================
// PRIVATE FUNCTIONS IMPLEMENTATION
// =============================================================================

void sysModulesInit(void) {
     IntAINTCInit();
     gpioInitModule(GPIO0);
     gpioInitModule(GPIO1);
     gpioInitModule(GPIO2);
     gpioInitModule(GPIO3);
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

void drvButtonInit(void) {
     btn.port = GPIO1;
     btn.pin_number = 16;
     gpioPInitPin(&btn, INPUT);

     gpioAintcConfigure(SYS_INT_GPIOINT1A, 0, irqHandlerGpio1A);
     gpioPinIntEnable(&btn, GPIO_INTC_LINE_1);
     gpioIntTypeSet(&btn, GPIO_INTC_TYPE_RISE_EDGE);
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
     uint8_t key = 0;

     gpioSetPinValue(&l1, LOW); 
     gpioSetPinValue(&l2, LOW); 
     gpioSetPinValue(&l3, LOW); 
     gpioSetPinValue(&l4, LOW); 

     gpioSetPinValue(&l1, HIGH);
     if(gpioGetPinValue(column)) {
          key = 1;
          goto ret;
     }    
     gpioSetPinValue(&l1, LOW);

     gpioSetPinValue(&l2, HIGH); 
     if(gpioGetPinValue(column)) {
          key = 2;
          goto ret;
     }    
     gpioSetPinValue(&l2, LOW); 

     gpioSetPinValue(&l3, HIGH); 
     if(gpioGetPinValue(column)) {
          key = 3;
          goto ret;
     } 
     gpioSetPinValue(&l3, LOW);   

     gpioSetPinValue(&l4, HIGH); 
     if(gpioGetPinValue(column)) {
          key = 4;
          goto ret;
     }   
     gpioSetPinValue(&l4, LOW); 

     ret:
     gpioSetPinValue(&l1, HIGH); 
     gpioSetPinValue(&l2, HIGH); 
     gpioSetPinValue(&l3, HIGH); 
     gpioSetPinValue(&l4, HIGH); 

     return key;
}

void drvDelayMs(int ms) {
     for(int i = 0; i < ms; i++) {
          for(int j = 0; j < 30000; j++);
     }
}