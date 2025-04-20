#include <SPI.h>

#define START_MESSAGE 254;
#define END_MESSAGE   255;

volatile uint8_t command[5];      // to store incoming packet
volatile uint8_t index = 0;      // current write index
volatile bool messageReady = false;

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);       // Required for SPI slave
  pinMode(53, INPUT);          // SS must be input or low

  SPCR |= _BV(SPE);            // Enable SPI in slave mode
  SPI.attachInterrupt();       // Enable SPI interrupt
}

// ISR for SPI data receive
ISR(SPI_STC_vect) {
  uint8_t data = SPDR;

  // return if the message is first but not START_MESSAGE 
  if (index == 0 && data != START_MESSAGE) return;

  command[index] = data;
  index++;

  if (index == 5 && command[4] == END_MESSAGE) {
    messageReady = true;
    index = 0; // reset for next message
  } else if (index >= 5) {
    // out of sync or some other problem, reset
    index = 0;
  }
}

void loop() {
  if (messageReady) {
    messageReady = false; //reset for next message

    uint8_t device_type = command[1];
    uint8_t device_pin  = command[2];
    uint8_t position    = command[3];

    Serial.println("Command Received:");
    Serial.print("  Device Type: "); Serial.println(device_type);
    Serial.print("  Device Pin:  "); Serial.println(device_pin);
    Serial.print("  Position:    "); Serial.println(position);

    //now do stuf with servo and pin 
  }
}
