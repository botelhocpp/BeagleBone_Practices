#include "stdcommon.h"

bool isNumber(char *str) {
     while(*str != '\0') {
          if(*str < '0' || *str > '9') {
               return false;
          }
          str++;
     }
     return true;
}