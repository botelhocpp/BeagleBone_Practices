/**
 * @file main.c
 * @author Pedro Botelho (pedrobotelho15@alu.ufc.br)
 * @brief 
 * @version 1.0
 * @date 2022-07-13
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
     
     /* Temporary Memory Space to Store a String to be Printed */
     char buffer[10];

     /* Size of the String to be Printed */
     uint8_t size = 0;

     /* Pressed Key of the Keyboard */
     int8_t key = -1;

     while(1) {
          /* Waits for the keyboard to be used. */
          /* Only read the keyboard if read flag is false. */
          if(drvCheckReadFlag()) {
               /* Check read flag again until it's false */
               continue;
          }

          /* Obtains the pressed key and signals that the keyboard has been read */
          key = drvReadPressedKey();

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
 * @brief The current pressed number register.
 */
static int32_t num = 0;

/**
 * @brief The current accumulator register.
 */
static int32_t acc = 0;

/**
 * @brief The current operation to be performed.
 */
static char operation = '+';

/**
 * @brief Signals wether the current input is a number or not. 
 */
static bool numberInput = false;

static uint8_t number_size = 0;

// =============================================================================
// PRIVATE FUNCTIONS IMPLEMENTATION
// =============================================================================

int getCurrentValue(int8_t key, char *buffer) {

     /* Result to be put to buffer (if it's not '-1') */
     int result = -1;

     /* Clear (C) Button: Clears all the operation */
     if(key == -1) {
          /* Signals that the current operation is not a number input */
          numberInput = false;

          /* Clears accumulator register */
          acc = 0;

          /* Clears current number register */
          num = 0;

          /* Value to be printed is 0 */
          result = 0;
     }

     /* TODO: Cancel Entry (CE) Button: Clears the most recent entry */

     /* Numeric Keys (0-9) */
     else if (key >= 0 && key <= 9) {

          /* Previous Input was not a Number */
          if(numberInput == false) {
               /* Current input is a number */
               numberInput = true;

               /* Update 'num' register */
               num = key;
          }

          /* Previous Input was a Number */
          else {
               /* Last operation was 'Equals', so clear Accumulator */
               if (operation == ' ') {
                    acc = 0;
               }

               /* Last operation was number input */
               else if(num != 0 && number_size > 0) {
                    int32_t temp = num*10 + key;
                    if(temp < 2147483648) {
                         num = temp;
                    }
               }
          }

          /* Obtains the result to be printed */
          result = num;
     }

     /* Sum Button (A) */
     else if (key == 10) {
          /* Current input is not a number */
          numberInput = false;

          /*  */
          if (acc == 0) {
               operation = '+';
               acc = num;
          }
          else if (num != 0) {
               operation = '+';
               acc += num;
               result = acc;
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
          numberInput = false;
          if (operation == '+') {
               operation = ' ';
               acc += num;
               num = 0;
               result = acc;
          }
          else if(operation == '-') {
               // TODO: SUB
          }
          else if(operation == '*') {
               // TODO: MUL
          }
          else {
               // TODO: DIV
          }
     }

     /* If the last pressed key resulted in a result */
     if(result != -1) {
          number_size = intToString(result, buffer, 10);
     }
     return number_size;
}

void printValue(char *buffer, uint8_t size) {
     /* Clears UART Terminal */
     putString("\033[H\033[J\r", 8);

     /* Print Current Value */
     putString(buffer, size);

     /* New Line */
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