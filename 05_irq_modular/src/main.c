/**
 * @file main.c
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @brief Basic interrupt program, where a button light
 * four embedded LEDs, using modularized functions.
 * @version 1.0
 * @date 2022-07-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "gpio.h"
#include "interrupt.h"

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

/**
 * @brief Initializes the components of the system, such as LEDs, buttons and
 * interrupts.
 */
void drvComponentInit(void);

/**
 * @brief Handles the interrupt request coming from GPIO1A interrupt line, toggling
 * the LED level.
 */
void irqHandlerGpio1A(void);

// =============================================================================
// MAIN CODE
// =============================================================================

int main(void){
     /* Disable IRQ in the CPSR */
     IntMasterIRQDisable();

     /* Initialize the LEDs and the Button */
     drvComponentInit();
     
     /* Enable IRQ in the CPSR */
     IntMasterIRQEnable();

     /* Block the CPU here */
     while(1) {
          gpioTogglePinValue(&led04);
          for(int i = 0; i < 500; i++) {
               for(int j = 0; j < 300000; j++);
          }
     }
}

// =============================================================================
// FUNCTION IMPLEMENTATION
// =============================================================================

void irqHandlerGpio1A(void) {
     /* Check if the IRQ was generated by the button */
	if(gpioCheckIntFlag(&btn, GPIO_INTC_LINE_1)) {

          /* Toggle the LED level */
          gpioTogglePinValue(&led01);
          gpioTogglePinValue(&led02);
          gpioTogglePinValue(&led03);

          /* Clear the pending status of the interrupt flags */
          gpioClearIntFlag(&btn, GPIO_INTC_LINE_1);
	}
}

void drvComponentInit(void) {
     /* Enable clock to GPIO1, enabling the module */
     gpioInitModule(GPIO1);

     /* Initialize the 4 embedded LEDs as OUTPUT */
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

     /* Initialize the external button in GPIO1_16 as input */
     btn.port = GPIO1;
     btn.pin_number = 16;
     gpioPInitPin(&btn, INPUT);
     
     /* Enable the IRQ for GPIO1A, with maximum priority and attach the ISR */
     gpioAintcConfigure(SYS_INT_GPIOINT1A, 0, irqHandlerGpio1A); 

     /* Enable the IRQ for the button pin, in GPIO1A*/
     gpioPinIntEnable(&btn, GPIO_INTC_LINE_1);

     /* Configure the IRQ trigger for the button pin */
     gpioIntTypeSet(&btn, GPIO_INTC_TYPE_RISE_EDGE);
}