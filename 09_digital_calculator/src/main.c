// =============================================================================
// LIBRARIES
// =============================================================================

#include <stdint.h>

#include "lcd.h"
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
int intToString(int32_t value, char *buffer, uint8_t size);

void calculationProcedureUart(char key);

void calculationProcedureLcd(char key);

// =============================================================================
// MAIN CODE
// =============================================================================

int main(void){

     /* Inicializa os componentes do sistema e drivers. */
     IntMasterIRQDisable();
     drvComponentInit();
     IntMasterIRQEnable();
     
     /* Limpa o terminal UART */
     clearTerminal();

     /* Valor Padrão Inicial da Calculadora */
     putCh('0');

     /* Tecla pressionada no teclado. */
     char key;

     while(1) {
          /* Verifica se o teclado foi lido. */
          if(drvCheckReadFlag()) {
               /* Se sim, verifique até o estado ser atualizado. */
               continue;
          }

          /* Se não, obtenha a nova tecla pressionada. */
          key = drvReadPressedKey();
          
          /* Execute um processamento baseado na tecla pressionada. */
          calculationProcedureUart(key);
     }
}

// =============================================================================
// PRIVATE VARIABLES
// =============================================================================

/* Espaços de memória auxiliares para converter int em str. */
static char buffer[20];
static int buffer_size = 0;

/* Índice do operando atual: */
/* 0: Primeiro               */
/* 1: Segundo                */
/* 2: Resultado              */
static int indice_operando = 0;

/* Operação Atual. */
static char operacao = ' ';

/* Operandos das operações. */
static long operando01 = 0;
static long operando02 = 0;

/* Resultado das Operações. */
/* Resultado = OP1 operacao OP2 */
static long resultado = 0;

// =============================================================================
// PRIVATE FUNCTIONS IMPLEMENTATION
// =============================================================================

void calculationProcedureUart(char key) {
     /* Verifica se a tecla pressionada é numérica */
     if (key >= '0' && key <= '9') {
          /* Verifica o índice do operando para modificar a variável correspondente */
          if (indice_operando == 0) {
               if(operando01 == 0) {
                    clearTerminal();
               }
               /* Coloque o numero pressionado na tela */
               putCh(key);

               /* Obtem o valor numerico da tecla */
               key -= '0';

               /* Concatena o valor anterior com a tecla escolhida */
               long temp = operando01 * 10 + key;
               if(temp < 2147483648) {
                    operando01 = temp;
               }
               else {
                    putString("Nao e possivel inserir numeros muito grandes!", 47);
               }
          }
          else if (indice_operando == 1) {
               putCh(key);
               key -= '0';
               long temp = operando02 * 10 + key;
               if(temp < 2147483648) {
                    operando02 = temp;
               }
               else {
                    putString("Nao e possivel inserir numeros muito grandes!", 47);
               }
          }

          /* Tecla de Igual (#) foi pressionada anteriormente, resetar o procedimento. */
          else {
               /* Limpa o terminal para inserir os novos valores iniciais */
               clearTerminal();

               /* Inicia a tela com a tecla pressionada */
               putCh(key);

               /* Reseta o indice do operando */
               indice_operando = 0;

               /* Limpa a operação atual */
               operacao = ' ';

               /* Obtém o primeiro operando como a tecla pressionada */
               operando01 = key - '0';
               operando02 = 0;
          }
     }

     /* Verifica se a Tecla Pressionada é o Botão Vermelho */
     else if (key == 'C') {
          /* Irá limpar tudo, inclusive o terminal */
          clearTerminal();

          /* Reseta as variáveis */
          operando01 = 0;
          operando02 = 0;
          indice_operando = 0;
          operacao = ' ';
          resultado = 0;

          /* Coloca zero no terminal para indicar que o sistema foi "limpo" */
          putCh('0');
     }

     /* Verifica se a Tecla Pressionada é a tecla 'A' (DELETE) */
     else if (key == 'D') {

          /* Limpa o terminal para reconstruírmos o estado anterior à entrada atual */
          clearTerminal();
          
          /* Se o botão foi pressionado durante a inserção do segundo operando */
          if (indice_operando == 1) {
               /* Reseta o valor do segundo operando */
               operando02 = 0;

               /* Transforma o operando 1 em uma string, salvando-a em buffer */
               buffer_size = intToString(operando01, buffer, 20);

               /* Imprime o operando 01 */  
               putString(buffer, buffer_size); 

               /* Imprime a operação */
               putCh(operacao);     

               /* Retorna o índice do operando para o anterior */
               indice_operando--;
          }
          
          /* Se o botão foi pressionado durante a escolha da operação */
          else if (operacao != ' ' && operando02 == 0) {
               /* Reseta a escolha de operação */
               operacao = ' ';

               /* Transforma o operando 01 dm string e salva em buffer */
               buffer_size = intToString(operando01, buffer, 20);

               /* Imprime o operando 01 */
               putString(buffer, buffer_size);
          }
          
          /* Se o botão foi pressionado durante a inserção do primeiro operando */
          else if (indice_operando == 0) {
               /* Reseta o valor do operando 01 */
               operando01 = 0;

               /* Reseta a operação */
               operacao = ' ';

               /* Mostra Zero no display */
               putCh('0');
          }
          // No resultado:
          else if(indice_operando == 2) {
               operando01 = 0;
               operando02 = 0;
               operacao = ' ';
               putCh('0');
          }
     }

     /* Verifica se a tecla igual foi pressionada durante a operação de soma */
     else if (key == '=' && operacao == '+') {
          /* Incrementa o operando para indicar "resultado" */
          indice_operando++;

          /* Limpa o identificador de operação */
          operacao = ' ';

          /* Realiza a operação entre os operandos e salva em resultado */
          resultado = operando01 + operando02;
          
          /* Imprime o resultado no ter
               putCh('0');minal */
          putString("\r\nANS=", 7);
          buffer_size = intToString(resultado, buffer, 20);
          putString(buffer, buffer_size);
     }

     /* Verifica se a tecla igual foi pressionada durante a operação de subtração */
     else if (key == '=' && operacao == '-') {
          indice_operando++;
          operacao = ' ';
          resultado = operando01 - operando02;
          putString("\r\nANS=", 7);

          /* Cria uma variável temporária para tratar do caso negativo */
          int valor_impresso = resultado;
          if (operando01 < operando02) {
               valor_impresso *= -1;
               putCh('-');
          }

          /* Imprime no terminal */
          buffer_size = intToString(valor_impresso, buffer, 20);
          putString(buffer, buffer_size);
     }

     /* Verifica se a tecla igual foi pressionada durante a operação de multiplicação */
     else if (key == '=' && operacao == '*') {
          indice_operando++;
          operacao = ' ';
          resultado = operando01 * operando02;
          putString("\r\nANS=", 7);
          buffer_size = intToString(resultado, buffer, 20);
          putString(buffer, buffer_size);
     }

     /* Verifica se a tecla igual foi pressionada durante a operação de divisão */
     else if (key == '=' && operacao == '/') {

     }

     /* Verifica se uma tecla de operação foi pressionada durante a inscerção do primeiro operando */
     else if ((key == '+' || key == '-' || key == '*' || key == '/') && (indice_operando == 0)) {
          /* O próximo operando será manejado */
          indice_operando++;

          /* A operação a ser realizada foi digitada */
          operacao = key;

          /* Imprime o simbolo da operação */
          putCh(operacao);
     }

     /* Verifica se uma tecla de operação foi pressionada após obter um resultado */
     else if ((key == '+' || key == '-' || key == '*' || key == '/') && (operacao == ' ')) {
          clearTerminal();

          /* Próxima entrada será o segundo operando */
          indice_operando = 1;
          
          /* O primeiro operando será o resultado obtido anteriormente */
          operando01 = resultado;

          /* Nova operação a ser realizada com o resultado obtido */
          operacao = key;

          /* Operando 02 vai ser preenchido pelo usuário */
          operando02 = 0;

          /* Imprime o operando 01 e a operação */
          buffer_size = intToString(operando01, buffer, 20);
          putString(buffer, buffer_size);
          putCh(operacao);
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
