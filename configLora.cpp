#include "configLora.h"

void loraSerialBufClear();
void loraSerialBufClearPrt();
void critErrorAct();
void loraTryHandshake();
void loraReset();
void loraConfigMode();

void loraConfig() {
  /*
  A function simply to make the setup() function prettier
  
  Configs Lora as follows:
    Baud Rate: 9600 bps
    LoraSerial Arguments:
      Data Bits: 8
      Stop Bits: 1
      Correction Bits: NONE
    Transmission Arguments:
      Channel: 25
      Transmission Power: 22dBm
      On Air Speed Rate: 4.8K
    Transmission Mode: Fix-point
    Pack Size: 20
    Sleep Time: 0
    Local Group: 25
    Local Addr: 25
    Targ Group: 25
    Targ Addr: 26
  */

  // Lora enter config mode
  loraConfigMode();

  // Handshake
  loraTryHandshake();

  // Reset
  loraReset();

  // This code is really shitty,
  // but yeah, run time efficiency is no.1 in embedded systems
  // goddamn lora api, how much do you need to preserve?
  const byte buf[] = {
    0x80, 0x04, 0x1E, //cmd, 0x80 write local success, return if error
    LORA_BAUDRATE,    //0x04
    LORA_SERIALARGS,  //0x05
    LORA_TRANSARGS,   //0x06
    LORA_WORKMODE,    //0x07
    0x00,0x00,0x00,   //0x08 to 0x09, preserved
    0x00,             //0x0A, default, main or follow machine, 0 for main
    0x77, 0x77, 0x77, 0x2E, 0x61, 0x73, 0x68, 0x69, 0x6E, 0x69, 0x6E, 0x67, 0x2E, 0x63, 0x6F, 0x6D, //0x0B, default, AES key
    0x00,0x00,0x00,0x00,0x00,0x00, //0x0C to 0x0D, preserved
    LORA_PACKSIZE,    //0x0E
    0x00,0x00,0x00,0x00,0x00,0x00,0x00, //0x0F to 0x13, preserved
    LORA_SLEEPTIME,   //0x14
    0x00,0x00,        //0x15 to 0x16, preserved
    0x00, 0x80,       //0x17, default, which is enable wireless wake code
    LORA_LOCGROUP,    //0x18
    LORA_LOCADDR,     //0x19
    LORA_TARGADDR,    //0x1A
    LORA_TARGADDR,    //0x1B
    0x00,0x00,0x00,0x00,0x00,0x00 //0x1C to 0x21, related to relay mode
  };
  //ASDS transmission
  //const byte buf[] = {0x80, 0x04, 0x1E, 0x00, 0x00, 0x25, 0x80, 0x00, 0x04, 0x1A, 0x00, 0x02, 0x05, 0x03, 0xE8, 0x00, 0x77, 0x77, 0x77, 0x2E, 0x61, 0x73, 0x68, 0x69, 0x6E, 0x69, 0x6E, 0x67, 0x2E, 0x63, 0x6F, 0x6D, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x05, 0x20, 0x00, 0x23, 0x00, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x0A, 0x19, 0x00, 0x00, 0x25, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02};

  LoraSerial.write(buf, 61);
  DebugSerial.print("INFO: sending config cmd\n\tres: ");

  int cnt = 0;
  int correctcnt = 0;
  while (true) {
    if (LoraSerial.available() > 0) {
      byte ret = LoraSerial.read();
      DebugSerial.print(ret);
      DebugSerial.print(", ");

      if (correctcnt >= 3) {
        break;
      }
      if (ret == buf[cnt]) {
        correctcnt++;
      } else {
        // Unknown return value
        DebugSerial.print("(wrong byte)");
        critErrorAct();
      }
      cnt++;
    }
  }
  DebugSerial.println();
  DebugSerial.println("INFO: lora config success");
  loraSerialBufClear();
}

void loraTryHandshake() {
  // return true for success, false for error
  // but it'll show up in debug anyway
  byte handshakemsg[] = {0, 0, 1};
  int cnt = 0;
  int correctcnt = 0;

  LoraSerial.write(handshakemsg, 3);
  DebugSerial.print("INFO: sending handshake, note that the program will wait forever\n\tres: ");
  while (true) {
    if (LoraSerial.available() > 0) {
      byte retbyte = LoraSerial.read();
      DebugSerial.print(retbyte);
      DebugSerial.print(", ");

      if (correctcnt >= 3) {
        break;
      }
      DebugSerial.print(retbyte);
      if (retbyte == handshakemsg[cnt]) {
        correctcnt++;
      } else {
        DebugSerial.print("(wrong byte)");
        critErrorAct();
      }
      cnt++;
    }
  }
  loraSerialBufClearPrt();
  DebugSerial.println();
  DebugSerial.println("INFO: handshake success");
}

void loraReset() {
  byte resetmsg[] = {0x80, 0x23, 0x01};
  byte expectret[] = {'O', 'K'};
  LoraSerial.write(resetmsg, 3);
  DebugSerial.print("INFO: sending reset cmd\n\tres: ");

  int cnt = 0;
  int correctcnt = 0;
  while (true) {
    if (LoraSerial.available() > 0) {
      byte retbyte = LoraSerial.read();
      DebugSerial.print(retbyte);
      DebugSerial.print(", ");

      if (correctcnt >= 2) {
        break;
      }
      if (retbyte == expectret[cnt]) {
        correctcnt++;
      } else {
        DebugSerial.print("(wrong byte)");
        //critErrorAct();
      }
      cnt++;
    }
  }
  DebugSerial.println();
  DebugSerial.println("INFO: reset success");
  loraSerialBufClear();
}

void loraConfigMode() {
  digitalWrite(PINMD0, LOW);
  digitalWrite(PINMD1, LOW);
  delay(120);
  DebugSerial.println("INFO: lora entered config mode");
}

void loraWorkMode() {
  digitalWrite(PINMD0, HIGH);
  digitalWrite(PINMD1, LOW);
  delay(120);
  DebugSerial.println("INFO: lora entered work mode");
}

void loraSerialBufClear() {
  while (LoraSerial.read() >= 0) {}
}
void loraSerialBufClearPrt() {
  while (LoraSerial.available() > 0) {
    DebugSerial.print((char)LoraSerial.read());
  }
}

void critErrorAct() {
  DebugSerial.println();
  DebugSerial.println("ERROR: met CRITICAL ERROR, entered loop");
  while (true) { delay(1000); }
}
