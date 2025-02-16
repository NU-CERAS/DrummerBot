#include "MIDIUSB.h"
#include <Servo.h>

// Define new neutral positions for each servo
const int neutPos[6] = {80, 87, 75, 76, 80, 101};

// Define hit positions (Kal servos: +5, Dal servos: -5)
const int hitPos[6] = {85, 82, 80, 81, 75, 106};

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
  return constrain(velocityControlByte, 40, 120);
}

int velocityControl(int changedVelocityControlByte, int servoIndex) {
  if (servoTypes[servoIndex] == 1) {  // Dal servos
    return neutPos[servoIndex] - ((changedVelocityControlByte - 40) * (neutPos[servoIndex] - maxVelDal) / 80);
  } else {                            // Kal servos
    return neutPos[servoIndex] + ((changedVelocityControlByte - 40) * (neutPos[servoIndex] - maxVelKal) / 80);
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
      int midiValue = rx.byte2;

      if (midiValue == MKK) {
        if (rx.header == 9) {  // Note On
          digitalWrite(KK1, HIGH);
          digitalWrite(KK2, HIGH);
        } else if (rx.header == 8) {  // Note Off
          digitalWrite(KK1, LOW);
          digitalWrite(KK2, LOW);
        }
      }
      else if (midiValue >= MD1 && midiValue <= MD6) {
        int servoIndex = midiValue - MD1;

        if (rx.header == 9) {
          servoValues[servoIndex] = velocityControl(adjustedVelocityControlByte(rx.byte3), servoIndex);
          servos[servoIndex].write(servoValues[servoIndex]);
          previousMillis[servoIndex] = currentMillis;
          servoAction[servoIndex] = true;
        } 
        else if (rx.header == 8) {
          servoValues[servoIndex] = neutPos[servoIndex];
          servos[servoIndex].write(servoValues[servoIndex]);
          servoAction[servoIndex] = false;
        }

        Serial.print(rx.byte1);
        Serial.print(" || ");
        Serial.print(rx.byte2);
        Serial.print(" || ");
        Serial.println(rx.byte3);
      }
    }
  } while (rx.header != 0);

  for (int i = 0; i < 6; i++) {
    if (servoAction[i] && (currentMillis - previousMillis[i] >= interval)) {
      servos[i].write(hitPos[i]);
      servoAction[i] = false;
    }
  }
}
