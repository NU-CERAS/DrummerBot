#include <SPI.h>

volatile byte receivedValues[4] = {0}; // order: start msg, device type, device id, position
volatile byte byteIndex = 0;
volatile bool ifComplete = false;

#define TYPE_SERVO
#define TYPE_STEPPER
#define TYPE_SOLENOID

// this is mega slave

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);       // Required for SPI slave
  pinMode(53, INPUT);          // SS must be input or low
  SPCR |= _BV(SPE);            // Enable SPI in slave mode
  SPI.attachInterrupt();       // Enable SPI interrupt
}

ISR(SPI_STC_vect) {
  receivedValue = SPDR;        // Store incoming byte

  // this code currently receives one byte, still need to:
    // if not start byte,
    // store it somewhere - array?
  if (byteIndex < 4) {
    receivedBytes[byteIndex++] = received; // <- increment happens here
    if (byteIndex == 4) {
      ifComplete = true;
    }
  }

}

void loop() {
  // once reaches 4 messages, adjust servos/motors
  if (receivedValues[1] == TYPE_SERVO) {
    // adjust servo
    // device id: receivedValues[2]
    // position: receivedValues[3]
  }
  else if (receivedValues[1] == TYPE_STEPPER) {
    // adjust stepper motor
  }
  else if (receivedValues[1] == TYPE_SOLENOID) {
    // adjust solenoid
  }

  Serial.println("Adjusted position");

  byteIndex = 0;
  ifComplete = false;

  delay(500);
}
