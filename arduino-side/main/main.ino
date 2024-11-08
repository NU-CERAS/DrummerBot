#include <Servo.h>
int val;
int dal;
int kal;
int val_init;
int dal_init;
int kal_init;

// Midi Constants
const int MKK = 36;
const int MD1 = 37;
const int MD2 = 38;
const int MD3 = 39;
const int MD4 = 40;
const int MD5 = 41;
const int MD6 = 42;

int KK1 = 4;
int KK2 = 7;
Servo D1;
Servo D2;
Servo D3;
Servo D4;
Servo D5;
Servo D6;


void setup() {
  Serial.begin(115200);
  pinMode(KK1, OUTPUT);
  pinMode(KK2, OUTPUT);
  D1.attach(3);
  D2.attach(5);
  D3.attach(6);
  D4.attach(9);
  D5.attach(10);
  D6.attach(11);
}

void loop() {

}
=======
#include "MIDIUSB.h"
#include <Servo.h>
int val;

// Midi Constants
const int MD1 = 36;
Servo D1;

int getDelay(vel) {
  if (vel < 50) {
    return 10;
  }
  else if (vel < 100) {
    return 50;
  }
  else if (vel < 150) {
    return 200;
  }
}

int getDelayDown(vel) {
  if (vel < 50) {
    return 7;
  }
  else if (vel < 100) {
    return 40;
  }
  else if (vel < 150) {
    return 170;
  }
}

int getPos(vel) {
  if (vel < 50) {
    return 120;
  }
  else if (vel < 100) {
    return 100;
  }
  else if (vel < 150) {
    return 60;
  }
}

int scheduled_next_event = -1;
int scheduled_next_next_event = -1;

void setup() {
  Serial.begin(115200);
  D1.attach(5);
}

void loop() {
  midiEventPacket_t rx;
  if (rx.header == 0x09) {
    //byte note = rx.byte2; 
    byte vel = rx.byte3;
    D1.write(getPos(vel));
    scheduled_next_event = millis() + getDelay(vel);
    scheduled_back_up_event = millis() + getDelay(vel) + getDelaydown(vel);
  }

  if (millis() >= scheduled_next_event) {
    D1.write(137);
    scheduled_down_event = -1;
  }

  if (millis() >= scheduled_back_up_event) {
    D1.write(125);
    scheduled_back_up_event = -1;
  }
}