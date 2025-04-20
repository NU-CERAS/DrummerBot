#include <AccelStepper.h>

const int stepsPerRevolution = 1000;  // Adjust this based on your stepper motor specifications
const int speed = 16000;                // Steps per second
const int acceleration = 8000;          // Acceleration

// Define stepper motor objects (AccelStepper uses DRIVER mode)
// The first pin is PULSE (WHITE), the second pin is DIRECTION (GREEN)
AccelStepper Stepper1(AccelStepper::DRIVER, 12, 13); // CENTRAL AXIS MOTOR
AccelStepper Stepper2(AccelStepper::DRIVER, 10, 11); // LEFT MOTOR
AccelStepper Stepper3(AccelStepper::DRIVER, 8, 9);// RIGHT MOTOR

void setup() {
  // Set speed and acceleration for all stepper motors
  Stepper1.setMaxSpeed(speed);
  Stepper1.setAcceleration(acceleration);

  Stepper2.setMaxSpeed(speed);
  Stepper2.setAcceleration(acceleration);

  Stepper3.setMaxSpeed(speed);
  Stepper3.setAcceleration(acceleration);

  // Initialize the serial monitor
  Serial.begin(9600);

  // Set target position (one full revolution) for motor 1 and 2, flip direction for motor 3
  Stepper1.moveTo(stepsPerRevolution);
  Stepper2.moveTo(stepsPerRevolution);
  Stepper3.moveTo(-stepsPerRevolution);  // Reversed direction for Stepper 3
}

void loop() {
  if (Stepper1.distanceToGo() == 0 && Stepper2.distanceToGo() == 0 && Stepper3.distanceToGo() == 0) {
    // Reverse direction when movement is complete
    Serial.println("Changing direction...");
    Stepper1.moveTo(-Stepper1.currentPosition());
    Stepper2.moveTo(-Stepper2.currentPosition());
    Stepper3.moveTo(-Stepper3.currentPosition());  // Reversed direction for Stepper 3
  }

  // Run all motors simultaneously
  Stepper1.run();
  Stepper2.run();
  Stepper3.run();
}
