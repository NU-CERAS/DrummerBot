#include "MIDIUSB.h"
#include <Servo.h>
// Define Servo Constants
const int neutPos = 65;
const int hitPos = 60;

// Define MIDI values for each servo
const int MD1 = 36;
const int MD2 = 37;
const int MD3 = 38;
const int MD4 = 39;
const int MD5 = 40;
const int MD6 = 41;

// Define servo pins
const int servoPins[] = {3, 5, 6, 9, 10, 11};
Servo servos[6];

// Define variables to store the current position and action state for each servo
int servoValues[6] = {0, 0, 0, 0, 0, 0};
unsigned long previousMillis[6] = {0, 0, 0, 0, 0, 0};  // Store timing for each servo
const long interval = 100;                              // Interval for servo movement in milliseconds
bool servoAction[6] = {false, false, false, false, false, false};  // Track action states for each servo


int adjustedVelocityControlByte(int velocityControlByte){
  if(velocityControlByte >= 120){
    return 120;
  }
  else if(velocityControlByte <= 40){
    return 40;
  }
  else{
    return velocityControlByte;
  }
}

int velocityControl(int changedVelocityControlByte){
    return (65 + (changedVelocityControlByte - 40)/2);
}

void setup() {
  Serial.begin(115200);

  // Attach each servo to its corresponding pin
  for (int i = 0; i < 6; i++) {
    servos[i].attach(servoPins[i]);
  }
}

void loop() {
  midiEventPacket_t rx;
  unsigned long currentMillis = millis();

  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      // Handle incoming MIDI messages
      int midiValue = rx.byte2;

      // Check each MIDI value to control the respective servo
      if (midiValue >= MD1 && midiValue <= MD6) {
        int servoIndex = midiValue - MD1;  // Calculate the servo index based on MIDI value

        if (rx.header == 9) {
          // Start servo action for the current servo
          servoValues[servoIndex] = velocityControl(adjustedVelocityControlByte(rx.byte3));
          servos[servoIndex].write(servoValues[servoIndex]); 
          previousMillis[servoIndex] = currentMillis;  // Initialize timing for the servo
          servoAction[servoIndex] = true;              // Mark action as active
        } 
        else if (rx.header == 8) {
          // Note-off message: set the servo to a neutral position
          servoValues[servoIndex] = neutPos;
          servos[servoIndex].write(servoValues[servoIndex]);
          servoAction[servoIndex] = false;             // Reset action on note-off
        }

        // Print MIDI details for debugging
        Serial.print(rx.byte1);
        Serial.print(" || ");
        Serial.print(rx.byte2);
        Serial.print(" || ");
        Serial.println(rx.byte3);
      }
    }
  } while (rx.header != 0);

  // Check each servo to see if it's time to move it back to the resting position
  for (int i = 0; i < 6; i++) {
    if (servoAction[i] && (currentMillis - previousMillis[i] >= interval)) {
      servoValues[i] = hitPos;          // Set resting position
      servos[i].write(servoValues[i]);
      servoAction[i] = false;        // Reset action after returning to resting position
    }
  }
}

