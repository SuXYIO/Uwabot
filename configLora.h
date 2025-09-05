#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include "pins.h"

void serialConfigReturnAct(byte);
void loraConfig();
void loraConfigSetSingleRegister(unsigned int, unsigned int);
void loraConfigMode();
void loraWorkMode();
void loraLowpowerMode();
void serialBufClear();

#endif
