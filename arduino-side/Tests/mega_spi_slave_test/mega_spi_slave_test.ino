#include <SPI.h>

#define START_MESSAGE 254;
#define END_MESSAGE   255;

void setup() {
  pinMode(10, OUTPUT);   // SS must be output
  SPI.begin();           // Start SPI
  Serial.begin(9600);
}

void moveServo(uint8_t device_type, uint8_t device_pin, uint8_t position) {
  SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
  digitalWrite(pin, LOW);

  SPI.transfer(START_MESSAGE);          // start message
  SPI.transfer(device_type);  // device type
  SPI.transfer(device_pin);   // specify device pin
  SPI.transfer(position);     // specify device pin
  SPI.transfer(END_MESSAGE);     // specify device pin

  digitalWrite(pin, HIGH);
  SPI.endTransaction();
}

void loop() {
  moveServo(0, 0, 120);
  delay(1000);
}
