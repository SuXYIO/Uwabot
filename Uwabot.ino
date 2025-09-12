#include "configLora.h"
#include "pins.h"

extern SoftwareSerial LoraSerial;
extern SoftwareSerial DebugSerial;

void setup() {
  // Init Lora
  // Setup
  pinMode(PINMD0, OUTPUT);
  pinMode(PINMD1, OUTPUT);
  pinMode(PINAUX, INPUT);
  LoraSerial.begin(9600);
  DebugSerial.begin(9600);

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
