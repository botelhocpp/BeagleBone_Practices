#include "uart.h"

#include "hw_types.h"

#define UART0_BASE 							0x44E09000
#define UART0_RHR  							0x44E09000
#define UART0_THR  							0x44E09000
#define UART0_LSR  							0x44E09014

void clearTerminal(void) {
     putString("\033[H\033[J\r", 8);
}

void putCh(char c){
	while(!(HWREG(UART0_LSR) & (1<<5)));
	HWREG(UART0_THR) = c;
}

char getCh(){
	while(!(HWREG(UART0_LSR) & (1<<0)));
	return HWREG(UART0_RHR);
}

int putString(char *str, unsigned int length){
	for(int i = 0; i < length; i++){
    	     putCh(str[i]);
	}
	return length;
}

int getString(char *buf, unsigned int length){
	for(int i = 0; i < length; i ++){
    		buf[i] = getCh();
   	}
	return length;
}