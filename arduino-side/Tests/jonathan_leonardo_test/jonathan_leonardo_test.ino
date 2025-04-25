#include "MIDIUSB.h"
#include <SPI.h>

#define SERVO_DEVICE_TYPE 1 // servo device types 
#define STEPPER_DEVICE_TYPE 2 // stepper device type 
#define KICK_DEVICE_TYPE 3 // Kick drum device type 
// Leonardo Master

// Define MIDI values for kick (These are the DeviceIDs)
const int MKK = 36;

// Define MIDI values for each servo
const int MD1 = 37;
const int MD2 = 38;
const int MD3 = 39;
const int MD4 = 40;
const int MD5 = 41;
const int MD6 = 42;
const int ServoID[6] = {37, 38, 39, 40, 41, 42}

// Define MIDI values for the Stepper Motors
const int ST1 = 43;
const int ST2 = 44;
const int ST3 = 45;

void setup() {
  pinMode(10, OUTPUT);   // SS must be output
  SPI.begin();           // Start SPI
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  midiEventPacket_t rx = MidiUSB.read();
  uint8_t MidiData[4] = {rx.header, rx.byte1, rx.byte2, rx.byte3};

  // Code that interprets the MIDI files, translates it into bytes to send to the Mega
  uint8_t StartMessage, DeviceType, DeviceID, Position;
  StartMessage = 0xFFFF; //Sets the header as 255
  if (rx.byte2 >= MD1 && rx.byte2 <= MD6){ // Checks if the signal is for a servo
    DeviceType = SERVO_DEVICE_TYPE;
    DeviceID = rx.byte2;
    if (rx.header == 9) {
      Position = rx.byte3;
    }
    else if (rx.header == 8) {
      Position = rx.byte3;
    }
  }
  else if (rx.byte2 == 36) { // Checks if the signal is for the kick 
    DeviceType = KICK_DEVICE_TYPE;
    DeviceID = rx.byte2;
  } 
  else if (rx.byte2 >= ST1 && rx.byte2 <= ST3){ // Checks if the signal is for Steppermotor
    DeviceType = STEPPER_DEVICE_TYPE;
    DeviceID = rx.byte2;
  }

  if (rx.header != 0){
    SPI.beginTransaction(SPISettings(500000, MSBFIRST,SPI_MODE0));
    digitalWrite(10, LOW);
    SPI.transfer(StartMessage);
    SPI.transfer(DeviceType);
    SPI.transfer(DeviceID);
    SPI.transfer(Position);
    digitalWrite(10,HIGH);
    SPI.endTransaction();
  

    Serial.println("Sent: ");
    Serial.print(StartMessage);
    Serial.print(" | ");
    Serial.print(DeviceType);
    Serial.print(" | ");
    Serial.print(DeviceID);
    Serial.print(" | ");
    Serial.print(Position);
  }
}
