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
