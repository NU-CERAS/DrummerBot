#include <SPI.h>

uint8_t info = 12;

void setup() {
  pinMode(10, OUTPUT);   // SS must be output
  SPI.begin();           // Start SPI
  Serial.begin(9600);
}

void loop() {
  SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
  digitalWrite(10, LOW);           // Select slave
  SPI.transfer(info);                // Send value
  digitalWrite(10, HIGH);          // Deselect
  SPI.endTransaction();

  Serial.println("Sent: info");
  delay(1000);
}
