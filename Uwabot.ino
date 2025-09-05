#include "configLora.h"
#include "pins.h"

void setup() {
  // Init Lora
  // Setup
  pinMode(PINMD0, OUTPUT);
  pinMode(PINMD1, OUTPUT);
  pinMode(PINAUX, INPUT);
  Serial.begin(9600, SERIAL_8N1);

  // Config Lora
  loraConfig();
  // Lora enters work mode automatically in loraConfig()

  // Use LED to indicate success
  digitalWrite(PINLED, HIGH);
  delay(500);
  digitalWrite(PINLED, LOW);
}

void loop() {
  ;
}
