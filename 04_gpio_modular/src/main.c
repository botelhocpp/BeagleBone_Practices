/**
 * @file main.c
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @brief Basic polling program, where a button light
 * four embedded LEDs, using modular functions.
 * @version 1.0
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "gpio.h"

// =============================================================================
// GPIO PINS
// =============================================================================

gpio_handle_t btn;
gpio_handle_t led01;
gpio_handle_t led02;
gpio_handle_t led03;
gpio_handle_t led04;

// =============================================================================
// FUNCTION PROTOTYPES
// =============================================================================

void drvComponentInit(void);

// =============================================================================
// MAIN CODE
// =============================================================================

int main(void){
     drvComponentInit();
     while(1) {
          if(gpioGetPinValue(&btn)) {
               gpioSetPinValue(&led01, HIGH);
               gpioSetPinValue(&led02, HIGH);
               gpioSetPinValue(&led03, HIGH);
               gpioSetPinValue(&led04, HIGH);
          }
          else {
               gpioSetPinValue(&led01, LOW);
               gpioSetPinValue(&led02, LOW);
               gpioSetPinValue(&led03, LOW);
               gpioSetPinValue(&led04, LOW);
          }
     }
}

// =============================================================================
// FUNCTION IMPLEMENTATION
// =============================================================================

void drvComponentInit(void) {
     gpioInitModule(GPIO1);

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

     btn.port = GPIO1;
     btn.pin_number = 12;
     gpioPInitPin(&btn, INPUT);
}
