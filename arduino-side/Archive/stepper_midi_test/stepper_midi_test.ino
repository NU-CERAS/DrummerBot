#include "MIDIUSB.h"

void setup() {
    Serial.begin(115200);
}

void loop() {
    midiEventPacket_t rx;
    
    do {
        rx = MidiUSB.read();
        if (rx.header != 0) {
            // Print raw MIDI data
            Serial.print("Header: ");
            Serial.print(rx.header, DEC);
            Serial.print(" | Byte 1: ");
            Serial.print(rx.byte1, DEC);
            Serial.print(" | Byte 2: ");
            Serial.print(rx.byte2, DEC);
            Serial.print(" | Byte 3: ");
            Serial.println(rx.byte3, DEC);
        }
    } while (rx.header != 0);
}
