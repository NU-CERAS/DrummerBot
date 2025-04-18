#include <SPI.h>

void setup() {
  pinMode(10, OUTPUT);   // SS must be output
  SPI.begin();           // Start SPI
  Serial.begin(9600);
}

void loop() {
  SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
  digitalWrite(10, LOW);           // Select slave
  SPI.transfer(42);                // Send value
  digitalWrite(10, HIGH);          // Deselect
  SPI.endTransaction();

  Serial.println("Sent: 42");
  delay(1000);
}
