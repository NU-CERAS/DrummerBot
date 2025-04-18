// Master Arduino (Receives midi signals from slave arduino)

#include "MIDIUSB.h"
#include <Servo.h>
#include <Wire.h>

#define SLAVE_ADDR 8

void setup() {
  Serial.begin(115200);
  Wire.begin();


}

void loop() {
  unsigned long currentMillis = millis();
  while(1) {
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.requestFrom(SLAVE_ADDR, 4);
    byte receivedData[4];  // Array to store incoming data
    for (int i = 0; i < 4 && Wire.available(); i++) {
        receivedData[i] = Wire.read();
    }
    Wire.endTransmission();
        // Print MIDI details for debugging
    Serial.print(receivedData[0]);
    Serial.print(" || ");
    Serial.print(receivedData[1]);
    Serial.print(" || ");
    Serial.print(receivedData[2]);
    Serial.print(" || ");
    Serial.println(receivedData[3]);
    delay(1000);
  } 

}

