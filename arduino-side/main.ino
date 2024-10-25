#include "MIDIUSB.h"
#include <Servo.h>
int val;
int dal;
int kal;

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

//functions that the servo number that the drumstick should rise to before hitting based on the velocity

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

int velocityControl(int servoNumber, int changedVelocityControlByte){
  if(servoNumber == MD1 || servoNumber == MD3 || servoNumber == MD4 || servoNumber == MD6){
    return (60 + (changedVelocityControlByte - 40)/2);
  }
  else if (servoNumber ==  MD2 || servoNumber == MD5){
    return (150 - 65((changedVelocityControlByte-40)/80));
  }
  else{
    return (120 - changedVelocityControlByte)/80;
  }
}

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
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      if (rx.header == 9) {
        val_init = velocityControl(rx.byte2, adjustedVelocityControlByte(rx.byte3));
        dal_init = velocityControl(rx.byte2, adjustedVelocityControlByte(rx.byte3));
        kal_init = velocityControl(rx.byte2, adjustedVelocityControlByte(rx.byte3));
        val = 60;
        dal = 150;
        kal = 1;
      }
      else if (rx.header == 8) {
        val = 100;
        dal = 85;
        kal = 0;
      }
      Serial.println(rx.byte2);
      switch (rx.byte2) {
        case MKK:
          digitalWrite(KK1, kal);
          digitalWrite(KK2, kal);
          break;
        case MD1:
          D1.write(val);
          break;
        case MD2:
          D2.write(dal);
          break;
        case MD3:
          D3.write(val);
          break;
        case MD4:
          D4.write(val);
          break;
        case MD5:
          D5.write(dal);
          break;
        case MD6:
          D6.write(val);
          break;
      }
    }
  } while (rx.header != 0);
}
