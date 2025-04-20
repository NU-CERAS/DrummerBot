#include <Servo.h>

const int Pos[6] = {68, 70, 69, 63, 78, 90};
const int servoPins[] = {2, 3, 4, 5, 6, 7};
Servo servos[6];

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 6; i++) {
    servos[i].attach(servoPins[i]);
  }
}

void loop() {
  for (int i = 0; i < 6; i++) {
    servos[i].write(Pos[i]);
  }
}
