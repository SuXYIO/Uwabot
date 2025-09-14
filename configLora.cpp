#include "configLora.h"
#include "pins.h"

void serialConfigReturnAct(int);
void loraConfigWriteReg(unsigned int, unsigned int, byte*);
void loraConfigMode();
void loraWorkMode();
void loraLowpowerMode();
byte* mergeByteArrays(byte*, unsigned int, byte*, unsigned int);
void serialBufClear();

void loraConfig() {
  /*
  A function simply to make the setup() function prettier
  Note that this function lets lora enter work mode automatically
  Configs Lora as follows:
    Baud Rate: 9600 bps
    LoraSerial Arguments:
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
  const byte bufBaudRate[4] = {0x00, 0x00, 0x25, 0x80};
  loraConfigWriteReg(0x04, 4, bufBaudRate);
  // LoraSerial Arguments
  /* bit5[stopBits = 1 (0b0)],
     bit4[frameLen = dataBits + correctionBits = 8 (0b0)],
     bit(2,1)[correctionBits = NONE (0b00)]
  therefore data = 0b000000 */
  const byte bufLoraSerialArgs[1] = {0x00};
  loraConfigWriteReg(0x05, 1, bufLoraSerialArgs);
  // Transmission Arguments
  /* bit(11,5)[channel = 20 (0b0010100)],
     bit(4,3)[power = 21dBm (0b11)],
     bit(2,0)[airSpeed = 4.8K (0b010)]
  therefore data = 0b001010011010 */
  const byte channel = LORA_CHANNEL;
  const byte b1 = channel >> 3;
  const byte b2 = channel << 5 + 0b11010;
  const byte bufTransArgs[2] = {b1, b2};
  loraConfigWriteReg(0x06, 2, bufTransArgs);
  // Transmission Mode
  /* 0x02 fix-point */
  const byte bufTransMode[2] = {0x00, 0x02};
  loraConfigWriteReg(0x07, 2, bufTransMode);
  // Pack Size
  const byte bufPackSize[1] = {20};
  loraConfigWriteReg(0x0E, 1, bufPackSize);
  // Sleep Time
  const byte bufSleepTime[1] = {0};
  loraConfigWriteReg(0x14, 1, bufSleepTime);
  // Local Addr
  const byte bufLocalAddr[1] = {LORA_ADDR};
  loraConfigWriteReg(0x19, 1, bufLocalAddr);

  // Lora enter work mode
  loraWorkMode();
}

void serialConfigReturnAct(int regAddr) {
  byte configReturn = LoraSerial.read();
  if (configReturn == 0x80) {
    // Config success
    serialBufClear();
    return;
  } else {
    // Unknown return value
    // probably Lora module not connected
    DebugSerial.print("ERROR: return: ");
    while (configReturn != -1) {
      DebugSerial.print(configReturn);
      configReturn = LoraSerial.read();
	  if (configReturn != -1)
		  DebugSerial.print(", ");
    }
    LoraSerial.print("\n");
    DebugSerial.print("addr: ");
    DebugSerial.print(regAddr);
    DebugSerial.print('\n');
	while (true) {}
  }
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
  const byte prefix[] = {0x80, regAddr, regLen};
  const byte *buf = mergeByteArrays(prefix, 3, data, regLen);
  LoraSerial.write(buf, 3 + regLen);
  free(buf);
  // I only read the first byte here, but should be enough to judge an error
  serialConfigReturnAct(regAddr);
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
  // LoraSerial.read() clears one byte, returns -1 if buf clear
  // Seems to be a hack but seems to be no better way
  while (LoraSerial.read() >= 0) {}
}
