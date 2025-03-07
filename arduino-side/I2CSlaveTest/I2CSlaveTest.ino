// Slave Arduino (Receives midi signals and sends to master)

#include "MIDIUSB.h"
#include <Servo.h>
#include <Wire.h>

#define SLAVE_ADDR 8

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(SLAVE_ADDR);
  Wire.onRequest(sendData);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void sendData() {
  midiEventPacket_t rx = MidiUSB.read();
  byte Data[4] = {rx.header, rx.byte1, rx.byte2, rx.byte3};
  Wire.write(Data,4);
}