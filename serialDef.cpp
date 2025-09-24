/*
Initialization of serials
to prevent multiple instances and weird bugs
*/

#include "pins.h"


SoftwareSerial LoraSerial(PINLORARX, PINLORATX);
// dunno why this doesn't work, only works with #define, putted that in pins.h
//SoftwareSerial DebugSerial(PINRX, PINTX);
