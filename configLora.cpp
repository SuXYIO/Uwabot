#include "configLora.h"
#include "pins.h"

void loraConfigWriteReg(unsigned int, unsigned int, byte*);
void loraConfigMode();
void loraWorkMode();
void loraLowpowerMode();
byte* mergeByteArrays(byte*, unsigned int, byte*, unsigned int);
void serialBufClear();

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
  byte bufBaudRate[4] = {0x00, 0x00, 0x25, 0x80};
  loraConfigWriteReg(0x04, 4, bufBaudRate);
  // Serial Arguments
  /* bit5[stopBits = 1 (0b0)],
     bit4[frameLen = dataBits + correctionBits = 8 (0b0)],
     bit(2,1)[correctionBits = NONE (0b00)]
  therefore data = 0b000000 */
  byte bufSerialArgs[1] = {0x00};
  loraConfigWriteReg(0x05, 1, bufSerialArgs);
  // Transmission Arguments
  /* bit(11,5)[channel = 20 (0b0010100)],
     bit(4,3)[power = 21dBm (0b11)],
     bit(2,0)[airSpeed = 4.8K (0b010)]
  therefore data = 0b001010011010 */
  byte bufTransArgs[2] = {0b0010, 0b10011010};
  loraConfigWriteReg(0x06, 2, bufTransArgs);
  // Transmission Mode
  /* 0x02 fix-point */
  byte bufTransMode[2] = {0x00, 0x02};
  loraConfigWriteReg(0x07, 2, bufTransMode);
  // Pack Size
  byte bufPackSize[1] = {20};
  loraConfigWriteReg(0x0E, 1, bufPackSize);
  // Sleep Time
  byte bufSleepTime[1] = {0};
  loraConfigWriteReg(0x14, 1, bufSleepTime);
  // Local Addr
  byte bufLocalAddr[1] = {20};
  loraConfigWriteReg(0x19, 1, bufLocalAddr);

  // Lora enter work mode
  loraWorkMode();
}

void loraConfigWriteReg(unsigned int regAddr, unsigned int regLen, byte data[]) {
  /*
  Write data to register one at a time
  write, local
  Enters serialConfigReturnAct() if error

  Example: set Fix-point Transmission mode
    => reg0x07 = 0x0002
    => Cmd[(write + local + success) + preserved] + Reg[0x07] + Len[1] + [Data0x0002]
    => 0b100 00000 + 0x07 + 0x01 + 0x0002
    => 0x80, 0x07, 0x02, {0x00, 0x02}
  */
  byte prefix[] = {0x80, regAddr, regLen};
  byte *buf = mergeByteArrays(prefix, 3, data, regLen);
  Serial.write(buf, 3 + regLen);
  free(buf);
  // I only read the first byte here, but should be enough to judge an error
  serialConfigReturnAct(Serial.read());
  serialBufClear();
}

byte* mergeByteArrays(byte arr1[], unsigned int l1, byte arr2[], unsigned int l2) {
	// remember to free() it yourself
	byte *merged = (byte*)malloc((l1 + l2) * sizeof(byte));

	for (int i=0; i < l1; i++)
		merged[i] = arr1[i];
	for (int i=0; i < l2; i++)
		merged[l1 + i] = arr2[i];

	return merged;
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
