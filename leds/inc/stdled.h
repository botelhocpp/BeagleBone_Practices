#ifndef STDLED_H
#define STDLED_H

void exportToFile(char *led, char *fileName, char *exportString);

void blinkLed(char *firstLed, char *lastLed, char *delayValue, int seconds);

void countToFifteen();

#endif