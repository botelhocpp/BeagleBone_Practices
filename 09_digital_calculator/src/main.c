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

// =============================================================================
// LIBRARIES
// =============================================================================

#include <stdint.h>

#include "gpio.h"
#include "interrupt.h"
#include "drivers.h"

// =============================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// =============================================================================

/**
 * @brief Convert a integer value to a string of given size, and save it in the
 * given buffer.
 * 
 * @param value The integer value to be converted.
 * @param buffer The memory buffer to place the resulting string. 
 * @param size The maximum size of the resulting string.
 * @return int The size of the resulting string. 
 */
int intToString(uint32_t value, char *buffer, uint8_t size);

/**
 * @brief Get the current value to be printed.
 * 
 * @param key The pressed key.
 * @param buffer A buffer to place the string to be printed.
 * @return int The size of the string.
 */
int getCurrentValue(int8_t key, char *buffer);

/**
 * @brief Print the value in the display.
 * 
 * @param buffer Where the value is, as a string.
 * @param size The size of the string.
 */
void printValue(char *buffer, uint8_t size);

// =============================================================================
// MAIN CODE
// =============================================================================

int main(void){

     /* Initialize the System Components and Drivers */
     IntMasterIRQDisable();
     drvComponentInit();
     IntMasterIRQEnable();
     
     char buffer[10];
     uint8_t size = 0;
     int8_t key = -1;

     while(1) {
          /* Waits for the keyboard to be used */
          while(drvGetReadFlag());

          /* Obtains the pressed key */
          key = drvLastPressedKey();

          /* Signals that the keyboard has been read */
          drvSetReadFlag();

          /* Get the current value to be shown */
          size = getCurrentValue(key, buffer);
          
          /* If it's not a number, don't do anything */
          if(size == 0) {
               continue;
          }

          /* Prints the current value on the screen */
          printValue(buffer, size);
     }
}

// =============================================================================
// PRIVATE VARIABLES
// =============================================================================

/**
 * @brief The current pressed number.
 */
static int32_t num = 0;

/**
 * @brief The current accumulator.
 */
static int32_t acc = 0;

/**
 * @brief The current operation to be performed.
 */
static char operation = '+';

// =============================================================================
// PRIVATE FUNCTIONS IMPLEMENTATION
// =============================================================================

int getCurrentValue(int8_t key, char *buffer) {
     int size = 0;

     /* Clear (C) Button: Clears all the operation */
     if(key == -1) {
          acc = 0;
          num = 0;
          size = intToString(num, buffer, 10);
     }

     /* TODO: Cancel Entry (CE) Button: Clears the most recent entry */

     /* Numeric Keys (0-9) */
     else if (key >= 0 && key <= 9) {
          if (operation == ' ') {
               acc = 0;
          }
          num = key;
          size = intToString(num, buffer, 10);
     }

     /* Sum Button (A) */
     else if (key == 10) {
          if (acc == 0) {
               operation = '+';
               acc = num;
          }
          else if (num != 0) {
               operation = '+';
               acc += num;
               size = intToString(acc, buffer, 10);
          }
          else {
               operation = '+';
          }
     }

     /* TODO: Sub Button (B) */

     /* TODO: Mul Button (C) */

     /* TODO: Div Button (D) */

     /* Equals Button (#) */
     else if (key == 15) {
          if (operation == '+') {
               operation = ' ';
               acc += num;
               num = 0;
               size = intToString(acc, buffer, 10);
          }
     }

     return size;
}

void printValue(char *buffer, uint8_t size) {
     putString("\033[H\033[J\r", 8);
     putString(buffer, size);
     putString("\n\r", 3);
}

int intToString(uint32_t value, char *buffer, uint8_t size) {
     char string[size];
     int i;
     for(i = 0; i < size - 1; i++) {
          string[i] = '0' + value % 10;
          value /= 10;
          if(value == 0) {
               break;
          }
     }
     int j;
     int a = i;
     for(j = 0; j <= i; j++) {
          buffer[j] = string[a--];
     }
     buffer[j++] = '\0';
     return j;
}