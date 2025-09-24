#pragma once
#include "pins.h"
#include <Arduino.h>

/*
Functions for configuring Lora module

Before me, the team used AS_DS.exe to configure Lora via a computer GUI,
but although easier, I think it's quite time wasting, since you have to connect the cables every time.
Since there are write commands in the docs, I implemented these, which lets arduino configure it.
Dunno if the exchange in time and performance (size of binary code) is sensible,
I'm new to embedded systems after all.
So I won't be really surprised if one day, after adding a bunch of functionalities,
we'll have to delete this due to waste of memory and storage.

All code here is oriented for A39C-T400A22D1a lora module
*/

// I'll have bit of a hack here using macros, since it reduces memory,
// and not as unreadable as just giving a list of bytes and tell you it works.
// I'll put up examples to how the numbers are calculated though

// Baud Rate
#define LORA_BAUDRATE 0x00, 0x00, 0x25, 0x80
// LoraSerial Arguments
  /* bit5[stopBits = 1 (0b0)],
      bit4[frameLen = dataBits + correctionBits = 8 (0b0)],
      bit(2,1)[correctionBits = NONE (0b00)]
  therefore data = 0b000000 */
#define LORA_SERIALARGS 0x00
// Transmission Arguments
  /* bit(11,5)[channel = 20 (0b0010100)],
      bit(4,3)[power = 21dBm (0b11)],
      bit(2,0)[airSpeed = 4.8K (0b010)]
  therefore data = 0b001010011010 */
#define LORA_CHANNEL 25
#define LORA_TRANSARGS LORA_CHANNEL >> 3, (byte)(LORA_CHANNEL << 5) + 0b11010
// Transmission Mode
// 0x01 for transparent, 0x02 for fix-point, others in the docs
#define LORA_WORKMODE 0x00, 0x02
// Pack Size
#define LORA_PACKSIZE 20
// Sleep Time
#define LORA_SLEEPTIME 0
// Local Group & Addr
#define LORA_LOCGROUP 25
#define LORA_LOCADDR 25
// Target Group & Addr
#define LORA_TARGGROUP 25
#define LORA_TARGADDR 26

// Others
#define LORA_HANDSHAKE_TIMEOUT 10000

void loraConfig();
void loraWorkMode();
