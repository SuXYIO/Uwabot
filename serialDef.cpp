/*
Initialization of serials
to prevent multiple instances and weird bugs
*/

#include "pins.h"

SoftwareSerial LoraSerial(PINLORARX, PINLORATX);
SoftwareSerial DebugSerial(PINRX, PINTX);
