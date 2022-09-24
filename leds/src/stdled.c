#include "stdled.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void exportToFile(char *led, char *fileName, char *exportString) {
     // Get the path to the LED:
     char beagleboneLed[38] = "/sys/class/leds/beaglebone:green:usr";
     strcat(beagleboneLed, led);

     // Get the string to export (to the file):
     char targetFile[48];
     strcpy(targetFile, beagleboneLed);
     strcat(targetFile, fileName);

     // Writes the string to file:
     FILE *filePointer = fopen(targetFile, "w");
     fwrite(exportString, 1, strlen(exportString), filePointer);
     fclose(filePointer);    
}

void blinkLed(char *firstLed, char *lastLed, char *delayValue, int seconds) {
     char noneTrigger[] = "none";
     char triggerTimer[] = "timer";
     char fileTrigger[] = "/trigger";
     char fileDelayOff[] = "/delay_off";

     char ledToBlink[] = "0";
     for(int i = atoi(firstLed); i <= atoi(lastLed); i++) {
          ledToBlink[0] = i + '0';
          exportToFile(ledToBlink, fileTrigger, noneTrigger);
          exportToFile(ledToBlink, fileTrigger, triggerTimer);
          exportToFile(ledToBlink, fileDelayOff, delayValue);
     }
     usleep(atoi(delayValue)*seconds*1000);

     for(int i = atoi(firstLed); i <= atoi(lastLed); i++) {
          ledToBlink[0] = i + '0';
          exportToFile(ledToBlink, fileTrigger, noneTrigger);
     }
}

void countToFifteen() {
     char noneTrigger[] = "none";
     char defaultOnTrigger[] = "default-on";
     char defaultOffTrigger[] = "oneshot";
     char fileTrigger[] = "/trigger";

     
     exportToFile("0", fileTrigger, noneTrigger);
     exportToFile("1", fileTrigger, noneTrigger);
     exportToFile("2", fileTrigger, noneTrigger);
     exportToFile("3", fileTrigger, noneTrigger);

     char ledToBlink[] = "0";
     for(int i = 0; i < 16; i++) {
          for(int j = 0; j <= 3; j++) {
               ledToBlink[0] = j + '0';
               if((i >> j) & 1) {
                    exportToFile(ledToBlink, fileTrigger, defaultOnTrigger);
               }
               else {
                    exportToFile(ledToBlink, fileTrigger, defaultOffTrigger);
               }
          }
          usleep(1000000);
     }

     exportToFile("0", fileTrigger, noneTrigger);
     exportToFile("1", fileTrigger, noneTrigger);
     exportToFile("2", fileTrigger, noneTrigger);
     exportToFile("3", fileTrigger, noneTrigger);
}