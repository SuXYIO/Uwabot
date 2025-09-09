#ifndef PINS_H
#define PINS_H

#define PINMD0 2
#define PINMD1 3
#define PINAUX 4

#define PINLORARX 5
#define PINLORATX 6

#define PINRX 0
#define PINTX 1

#define PINLED 13

#include <SoftwareSerial.h>
const SoftwareSerial LoraSerial(PINLORARX, PINLORATX);
const SoftwareSerial DebugSerial(PINRX, PINTX);

#endif
