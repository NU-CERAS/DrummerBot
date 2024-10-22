#include "MIDIUSB.h"
#include <Servo.h>
int val;
int dal;
int kal;

// Midi Constants
const int MKK = 36;
const int MD1 = 49;
const int MD2 = 38;
const int MD3 = 45;
const int MD4 = 48;
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

void setup()
{
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
    if (Serial.available() >= 6)
    {
        unsigned long time;
        byte note, velocity;

        Serial.readBytes((char *)&time, 4);
        note = Serial.read();
        velocity = Serial.read();

        // Wait until it's time to play this note
        while (millis() < time)
        {
            // Do nothing, just wait
        }

        // Play the note
        playNote(note, velocity);
    }
}

void playNote(byte note, byte velocity) {
    int val = map(velocity, 0, 127, 0, 180); // Map MIDI velocity to servo angle

    switch (note) {
    case MKK:
        digitalWrite(KK1, HIGH);
        digitalWrite(KK2, HIGH);
        delay(50); // Adjust this delay as needed
        digitalWrite(KK1, LOW);
        digitalWrite(KK2, LOW);
        break;
    case MD1:
        D1.write(val);
        break;
    case MD2:
        D2.write(val);
        break;
    case MD3:
        D3.write(val);
        break;
    case MD4:
        D4.write(val);
        break;
    case MD5:
        D5.write(val);
        break;
    case MD6:
        D6.write(val);
        break;
    }
}