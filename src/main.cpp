#include <Arduino.h>

extern void connectToNetwork();

void setup() {

  Serial.begin(9600);

  connectToNetwork();
}

void loop() {
  // put your main code here, to run repeatedly:
}