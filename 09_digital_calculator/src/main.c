// =============================================================================
// LIBRARIES
// =============================================================================

#include <stdint.h>

#include "lcd.h"
#include "uart.h"
#include "interrupt.h"
#include "drivers.h"

// =============================================================================
// PRIVATE GLOBAL CONTROL VARIABLES
// =============================================================================

lcd_handler_t *lcd;

// =============================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// =============================================================================

int intToString(int32_t value, char *buffer, uint8_t size);

void calculationProcedure(char key);

void debug(void);

// =============================================================================
// MAIN CODE
// =============================================================================

int main(void){

     IntMasterIRQDisable();
     drvComponentInit();
     IntMasterIRQEnable();

     lcd = drvGetLcdHandler();

     lcdClearDisplay(lcd);
     lcdSetCursor(lcd, 0, 0);

     
     lcdWriteChar(lcd, '0');

     char key;

     while(1) {
          if(drvCheckReadFlag()) {
               continue;
          }

          key = drvReadPressedKey();
          
          calculationProcedure(key);
     }
}

// =============================================================================
// PRIVATE VARIABLES
// =============================================================================

static char buffer[20];
static int buffer_size = 0;

static int indice_operando = 0;

static char operacao = ' ';

static long operando01 = 0;
static long operando02 = 0;

static long resultado = 0;

// =============================================================================
// PRIVATE FUNCTIONS IMPLEMENTATION
// =============================================================================

void debug(void) {
     putString("Operando 01: ", 14);
     int temp1 = operando01;
     if (operando01 < 0) {
          temp1 *= -1;
          putCh('-');
     }
     buffer_size = intToString(temp1, buffer, 20);
     putString(buffer, buffer_size);
     putString("\n\r", 3);

     putString("Operando 02: ", 14);
     int temp2 = operando02;
     if (operando02 < 0) {
          temp2 *= -1;
          putCh('-');
     }
     buffer_size = intToString(temp2, buffer, 20);
     putString(buffer, buffer_size);
     putString("\n\r", 3);

     putString("Resultado: ", 12);
     int tempr = resultado;
     if (resultado < 0) {
          tempr *= -1;
          putCh('-');
     }
     buffer_size = intToString(tempr, buffer, 20);
     putString(buffer, buffer_size);
     putString("\n\r", 3);
}

void calculationProcedure(char key) {
     if (key >= '0' && key <= '9') {
          if (indice_operando == 0) {
               if(operando01 == 0) {   
                    lcdSetCursor(lcd, 0, 0);
                    lcdClearDisplay(lcd);
               }
               lcdWriteChar(lcd, key);

               key -= '0';

               operando01 = operando01 * 10 + key;
          }
          else if (indice_operando == 1) {
               lcdWriteChar(lcd, key);
               key -= '0';
               operando02 = operando02 * 10 + key;
          }

          /* Tecla de Igual (#) foi pressionada anteriormente, resetar o procedimento. */
          else {
               lcdSetCursor(lcd, 0, 0);
               lcdClearDisplay(lcd);

               lcdWriteChar(lcd, key);

               indice_operando = 0;

               operacao = ' ';

               /* Obtém o primeiro operando como a tecla pressionada */
               operando01 = key - '0';
               operando02 = 0;
          }

     }

     /* Verifica se a Tecla Pressionada é o Botão Vermelho */
     else if (key == 'C') {
          lcdSetCursor(lcd, 0, 0);
          lcdClearDisplay(lcd);

          operando01 = 0;
          operando02 = 0;
          indice_operando = 0;
          operacao = ' ';
          resultado = 0;

          lcdWriteChar(lcd, '0');
     }

     /* Verifica se a Tecla Pressionada é a tecla 'A' (DELETE) */
     else if (key == 'D') {
          lcdSetCursor(lcd, 0, 0);
          lcdClearDisplay(lcd);
          
          /* Se o botão foi pressionado durante a inserção do segundo operando */
          if (indice_operando == 1 && operando02 != 0) { 
               operando02 = 0;
               int valor_impresso = operando01;
               if (operando01 < 0) {
                    valor_impresso *= -1;
                    lcdWriteChar(lcd, '-');
               }

               buffer_size = intToString(valor_impresso, buffer, 20);
               lcdWriteString(lcd, buffer);
               lcdWriteChar(lcd, operacao);   
          }
          
          /* Se o botão foi pressionado durante a escolha da operação */
          else if (operacao != ' ' && operando02 == 0) { 
               operacao = ' ';
               int valor_impresso = operando01;
               if (resultado < 0) {
                    valor_impresso *= -1;
                    lcdWriteChar(lcd, '-');
               }
               buffer_size = intToString(valor_impresso, buffer, 20);
               lcdWriteString(lcd, buffer);
          }
          
          /* Se o botão foi pressionado durante a inserção do primeiro operando */
          else if (indice_operando == 0) {
               operando01 = 0;
               operacao = ' ';
               lcdWriteChar(lcd, '0'); 
          }

          /* No resultado: */
          else if(indice_operando == 2) {
               operando01 = 0;
               operando02 = 0;
               resultado = 0;
               operacao = ' ';
               lcdWriteChar(lcd, '0'); 
          }
     }

     else if (key == '=' && operacao == '+') {
          /* Incrementa o operando para indicar "resultado" */
          indice_operando++;

          operacao = ' ';
          
          if(operando02 == 0) {
               lcdWriteChar(lcd, '0');
          }

          lcdWriteChar(lcd, '=');

          resultado = operando01 + operando02;
          
          int valor_impresso = resultado;
          if (resultado < 0) {
               valor_impresso *= -1;
               lcdWriteChar(lcd, '-');
          }
          buffer_size = intToString(valor_impresso, buffer, 20);
          lcdWriteString(lcd, buffer);
     }

     else if (key == '=' && operacao == '-') {
          indice_operando++;
          operacao = ' ';
          
          if(operando02 == 0) {
               lcdWriteChar(lcd, '0');
          }

          lcdWriteChar(lcd, '=');

          resultado = operando01 - operando02;

          int valor_impresso = resultado;
          if (resultado < 0) {
               valor_impresso *= -1;
               lcdWriteChar(lcd, '-');
          }
          buffer_size = intToString(valor_impresso, buffer, 20);
          lcdWriteString(lcd, buffer);
     }

     else if (key == '=' && operacao == '*') {
          indice_operando++;
          operacao = ' ';

          if(operando02 == 0) {
               lcdWriteChar(lcd, '0');
          }

          lcdWriteChar(lcd, '=');

          resultado = operando01 * operando02;

          int valor_impresso = resultado;
          if (resultado < 0) {
               valor_impresso *= -1;
               lcdWriteChar(lcd, '-');
          }

          buffer_size = intToString(valor_impresso, buffer, 20);
          lcdWriteString(lcd, buffer);
     }

     else if (key == '=' && operacao == '/') {
          
     }

     /* Verifica se uma tecla de operação foi pressionada durante a inscerção do primeiro operando */
     else if ((key == '+' || key == '-' || key == '*' || key == '/') && (indice_operando == 0)) {
          
          indice_operando++;

          operacao = key;

          lcdWriteChar(lcd, operacao);
     }

     /* Verifica se uma tecla de operação foi pressionada após obter um resultado */
     else if ((key == '+' || key == '-' || key == '*' || key == '/') && (operacao == ' ')) {
          lcdSetCursor(lcd, 0, 0);
          lcdClearDisplay(lcd);

          indice_operando = 1;
          
          operando01 = resultado;

          operacao = key;

          operando02 = 0;

          int valor_impresso = operando01;
          if (operando01 < 0) {
               valor_impresso *= -1;
               lcdWriteChar(lcd, '-');
          }

          buffer_size = intToString(valor_impresso, buffer, 20);
          lcdWriteString(lcd, buffer);
          lcdWriteChar(lcd, operacao);
     }
}

int intToString(int32_t value, char *buffer, uint8_t size) {
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