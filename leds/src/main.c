#include "stdcommon.h"
#include "stdled.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

     if(argc > 5) {
          printf("\033[1;37mleds:\033[0m \033[1;31mFatal error\033[0m: too much arguments.\nAborting.\n");
          return 0;
     }
     else if((argc == 2) && (strcmp(argv[1], "--help") == 0)) {
          printf("Usage: leds [options]\nOptions:\n  -l <LED range>\t\tThe range of LEDs to blink, from 0 to 3.\n\t\t\t\tCan specify one, or a range, <inital LED>-<final LED>,\n\t\t\t\tsuch as the final LED number is bigger or\n\t\t\t\tequal to the inital's LED number.\n  -d <delay time>\t\tThe time of one blink.\n  -t <time>\t\t\tThe time (in seconds) of the blink.\n  --version\t\t\tShow the version of the software.\n  --count\t\t\tCount from 0 to 15.\n");
          return 0;
     }
     else if((argc == 2) && (strcmp(argv[1], "--version") == 0)) {
          printf("leds 1.2.0\nCopyright (C) 2022 Pedro Botelho\n");
          return 0;
     }
     else if((argc == 2) && (strcmp(argv[1], "--count") == 0)) {
          countToFifteen();
          return 0;
     }

     char firstLed[] = "0";
     char lastLed[] = "3";
     char delayValue[10] = "1000";
     int seconds = 5;

     char *token;
     for(int i = 1; i < argc; i++) {
          if(strcmp(argv[i], "-l") == 0) {
               token = strtok(argv[i + 1], "-");
               if(token != NULL) {
                    if(!isNumber(token)) {
                         printf("\033[1;37mleds:\033[0m \033[1;31mFatal error\033[0m: the value for 'first LED' is not an integer number.\nAborting.\n");
                         return 0;
                    }
                    strcpy(firstLed, token);
                    token = strtok(NULL, "-");
                    if(token != NULL) {
                         if(!isNumber(token)) {
                              printf("\033[1;37mleds:\033[0m \033[1;31mFatal error\033[0m: the value for 'last LED' is not an integer number.\nAborting.\n");
                              return 0;
                         }
                         strcpy(lastLed, token);
                    }
                    else {
                         lastLed[0] = firstLed[0];
                    }
               }
               if ((atoi(firstLed) < 0) || (atoi(lastLed) > 3) || (atoi(firstLed) > atoi(lastLed))) {
                    printf("\033[1;37mleds:\033[0m \033[1;31mFatal error\033[0m: invalid range.\nAborting.\n");
                    return 0;
               }
          }
          else if(strcmp(argv[i], "-d") == 0) {
               if(!isNumber(argv[i + 1])) {
                    printf("\033[1;37mleds:\033[0m \033[1;31mFatal error\033[0m: the value for 'delay' is not an integer number.\nAborting.\n");
                    return 0;
               }
               strcpy(delayValue, argv[i + 1]);
          }
          else if(strcmp(argv[i], "-t") == 0) {
               if(!isNumber(argv[i + 1])) {
                    printf("\033[1;37mleds:\033[0m \033[1;31mFatal error\033[0m: the value for 'seconds' is not an integer number.\nAborting.\n");
                    return 0;
               }
               seconds = atoi(argv[i + 1]);
          }
     }
     blinkLed(firstLed, lastLed, delayValue, seconds);
}