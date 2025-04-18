#include "MIDIUSB.h"
#include <Servo.h>

// Define Servo Constants for Kal Servos (MD1, MD3, MD4, MD6)
const int neutPosKal = 110;
const int hitPosKal = 115;

// Define Servo Constants for Dal Servos (MD2, MD5)
const int neutPosDal = 65;
const int hitPosDal = 60;

const int maxVelDal = 125;
const int maxVelKal = 160;

// Define MIDI values for each servo
const int MKK = 36;
const int MD1 = 37;
const int MD2 = 38;
const int MD3 = 39;
const int MD4 = 40;
const int MD5 = 41;
const int MD6 = 42;

// Define kick drum pins 
const int KK1 = 4;
const int KK2 = 7;

// Define servo pins
const int servoPins[] = {3, 5, 6, 9, 10, 11};
Servo servos[6];

// Define variables to store the current position and action state for each servo
int servoValues[6] = {0, 0, 0, 0, 0, 0};
unsigned long previousMillis[6] = {0, 0, 0, 0, 0, 0};  // Store timing for each servo
const long interval = 20;                              // Interval for servo movement in milliseconds
bool servoAction[6] = {false, false, false, false, false, false};  // Track action states for each servo

// Define servo types: 0 for Kal, 1 for Dal
const int servoTypes[6] = {0, 1, 0, 0, 1, 0};  // MD2 and MD5 are Dal, others are Kal

int adjustedVelocityControlByte(int velocityControlByte) {
  if (velocityControlByte >= 120) {
    return 120;
  } else if (velocityControlByte <= 40) {
    return 40;
  } else {
    return velocityControlByte;
  }
}

int velocityControl(int changedVelocityControlByte, int servoType) {
  if (servoType == 1) {  // Dal servos
    // Scale velocity inversely for Dal servos: 145 (low velocity) to 85 (high velocity)
    return neutPosDal - ((changedVelocityControlByte - 40) * (neutPosDal - maxVelDal) / 80); // 80 = (120 - 40)
  } else {               // Kal servos
    // Scale velocity normally for Kal servos: 65 (low velocity) to 100 (high velocity)
    return neutPosKal + ((changedVelocityControlByte - 40) * (neutPosKal - maxVelKal) / 80); // 80 = (120 - 40)
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(KK1, OUTPUT);
  pinMode(KK2, OUTPUT);

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

      // Check for kick drum MIDI messages
      if (midiValue == MKK) {
        if (rx.header == 9) {  // Note On
          digitalWrite(KK1, HIGH);
          digitalWrite(KK2, HIGH);
        } else if (rx.header == 8) {  // Note Off
          digitalWrite(KK1, LOW);
          digitalWrite(KK2, LOW);
        }
      }
      // Check each MIDI value to control the respective servo
      else if (midiValue >= MD1 && midiValue <= MD6) {
        int servoIndex = midiValue - MD1;  // Calculate the servo index based on MIDI value
        int servoType = servoTypes[servoIndex];  // Determine servo type (Kal or Dal)

        if (rx.header == 9) {
          // Start servo action for the current servo
          servoValues[servoIndex] = velocityControl(adjustedVelocityControlByte(rx.byte3), servoType);
          servos[servoIndex].write(servoValues[servoIndex]);
          previousMillis[servoIndex] = currentMillis;  // Initialize timing for the servo
          servoAction[servoIndex] = true;              // Mark action as active
        } 
        else if (rx.header == 8) {
          // Note-off message: set the servo to a neutral position
          servoValues[servoIndex] = (servoType == 1) ? neutPosDal : neutPosKal;
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
    int servoType = servoTypes[i];  // Determine servo type
    if (servoAction[i] && (currentMillis - previousMillis[i] >= interval)) {
      servoValues[i] = (servoType == 1) ? hitPosDal : hitPosKal;  // Set resting position based on servo type
      servos[i].write(servoValues[i]);
      servoAction[i] = false;        // Reset action after returning to resting position
    }
  }
}