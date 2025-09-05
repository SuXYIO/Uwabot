#include "configLora.h"
#include "pins.h"

void serialConfigReturnAct(byte configReturn) {
  pinMode(PINLED, OUTPUT);
  if (configReturn == 0x80) {
    // Config success
    // Do nothing
    return;
  } else if (configReturn == 0xA0) {
    // Config fail
    // LED --__--__
    while(true) {
      digitalWrite(PINLED, HIGH);
      delay(500);
      digitalWrite(PINLED, LOW);
      delay(500);
    }
  } else {
    // Unknown return value
    // probably Lora module not connected
    // LED -_-__-_-__
    while(true) {
      digitalWrite(PINLED, HIGH);
      delay(200);
      digitalWrite(PINLED, LOW);
      delay(200);
      digitalWrite(PINLED, HIGH);
      delay(200);
      digitalWrite(PINLED, LOW);
      delay(400);
    }
  }
}

void loraConfig() {
  /*
  A function simply to make the setup() function prettier
  Note that this function lets lora enter work mode automatically
  Configs Lora as follows:
    Baud Rate: 9600 bps
    Serial Arguments:
      Data Bits: 8
      Stop Bits: 1
      Correction Bits: NONE
    Transmission Arguments:
      Channel: 20
      Transmission Power: 22dBm
      On Air Speed Rate: 4.8K
    Transmission Mode: Fix-point
    Pack Size: 20
    Sleep Time: 0
    Local Addr: 20
  */

  // Lora enter config mode
  loraConfigMode();
  // Baud Rate
  loraConfigSetSingleRegister(0x04, 9600);
  // Serial Arguments
  /* bit5[stopBits = 1 (0b0)],
     bit4[frameLen = dataBits + correctionBits = 8 (0b0)],
     bit(2,1)[correctionBits = NONE (0b00)]
  therefore data = 0b000000 */
  loraConfigSetSingleRegister(0x05, 0b0);
  // Transmission Arguments
  /* bit(11,5)[channel = 20 (0b0010100)],
     bit(4,3)[power = 21dBm (0b11)],
     bit(2,0)[airSpeed = 4.8K (0b010)]
  therefore data = 0b001010011010 */
  loraConfigSetSingleRegister(0x06, 0b001010011010);
  // Transmission Mode
  loraConfigSetSingleRegister(0x07, 0x02);
  // Pack Size
  loraConfigSetSingleRegister(0x0E, 20);
  // Sleep Time
  loraConfigSetSingleRegister(0x14, 0);
  // Local Addr
  loraConfigSetSingleRegister(0x19, 20);

  // Lora enter work mode
  loraWorkMode();
}

void loraConfigSetSingleRegister(unsigned int regAddr, unsigned int data) {
  /*
  Write data to register one at a time
  write, local
  Enters serialConfigReturnAct() if error

  Example: set Fix-point Transmission mode
    => reg0x07 = 0x0002
    => Cmd[(write + local + success) + preserved] + Reg[0x07] + Len[1] + [Data0x0002]
    => 0b100 00000 + 0x07 + 0x01 + 0x0002
    => 0x80, 0x07, 0x01, 0x02
  */
  byte buf[] = {0x80, regAddr, 0x01, data};
  Serial.write(buf, 4);
  // I only read the first byte here, but should be enough to judge an error
  serialConfigReturnAct(Serial.read());
  serialBufClear();
}

void loraConfigMode() {
  digitalWrite(PINMD0, LOW);
  digitalWrite(PINMD1, LOW);
}

void loraWorkMode() {
  digitalWrite(PINMD0, HIGH);
  digitalWrite(PINMD1, LOW);
}

void loraLowpowerMode() {
  digitalWrite(PINMD0, HIGH);
  digitalWrite(PINMD1, HIGH);
}

void serialBufClear() {
  // Serial.read() clears one byte, returns -1 if buf clear
  // Seems to be a hack but seems to be no better way
  while (Serial.read() >= 0) {}
}
