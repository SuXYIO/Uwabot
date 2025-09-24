#include "configLora.h"

extern SoftwareSerial LoraSerial;

void setup() {
  // Init Lora
  // Setup
  pinMode(PINMD0, OUTPUT);
  pinMode(PINMD1, OUTPUT);
  LoraSerial.begin(9600);
  DebugSerial.begin(9600);
  DebugSerial.println();

  // Config Lora
  loraConfig();

  // Lora enter work mode
  loraWorkMode();

  // Use LED to indicate success
  DebugSerial.println("INFO: Lora module setup complete");
}

void loop() {
  ;
}
