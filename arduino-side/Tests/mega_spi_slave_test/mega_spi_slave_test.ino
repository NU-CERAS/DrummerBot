#include <SPI.h>

volatile byte receivedValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);       // Required for SPI slave
  pinMode(53, INPUT);          // SS must be input or low
  SPCR |= _BV(SPE);            // Enable SPI in slave mode
  SPI.attachInterrupt();       // Enable SPI interrupt
}

ISR(SPI_STC_vect) {
  receivedValue = SPDR;        // Store incoming byte
}

void loop() {
  Serial.print("Received: ");
  Serial.println(receivedValue);
  delay(500);
}
