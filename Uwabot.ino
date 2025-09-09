#include "configLora.h"
#include "pins.h"

void setup() {
  // Init Lora
  // Setup
  pinMode(PINMD0, OUTPUT);
  pinMode(PINMD1, OUTPUT);
  pinMode(PINAUX, INPUT);
  LoraSerial.begin(9600);

  // Config Lora
  loraConfig();
  // Lora enters work mode automatically in loraConfig()

  // Use LED to indicate success
  DebugSerial.println("INFO: Lora module setup complete");
  digitalWrite(PINLED, HIGH);
}

void loop() {
  ;
}
