#include "MIDIUSB.h"

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for Serial Monitor to open
    Serial.println("MIDI Monitor Ready...");
}

void loop() {
    midiEventPacket_t rx;

    // Check if there's incoming MIDI data
    rx = MidiUSB.read();

    if (rx.header != 0) { // If a valid MIDI message is received
        Serial.print("MIDI Message Received - ");

        // Extract MIDI command type (upper 4 bits of byte1)
        byte command = rx.byte1 & 0xF0;
        byte channel = (rx.byte1 & 0x0F) + 1; // MIDI channels are 1-16

        switch (command) {
            case 0x80: // Note Off
                Serial.print("Note Off | ");
                Serial.print("Channel: "); Serial.print(channel);
                Serial.print(" | Note: "); Serial.print(rx.byte2);
                Serial.print(" | Velocity: "); Serial.println(rx.byte3);
                break;

            case 0x90: // Note On
                Serial.print("Note On  | ");
                Serial.print("Channel: "); Serial.print(channel);
                Serial.print(" | Note: "); Serial.print(rx.byte2);
                Serial.print(" | Velocity: "); Serial.println(rx.byte3);
                break;

            case 0xA0: // Aftertouch (Polyphonic Pressure)
                Serial.print("Poly Aftertouch | ");
                Serial.print("Channel: "); Serial.print(channel);
                Serial.print(" | Note: "); Serial.print(rx.byte2);
                Serial.print(" | Pressure: "); Serial.println(rx.byte3);
                break;

            case 0xB0: // Control Change
                Serial.print("Control Change | ");
                Serial.print("Channel: "); Serial.print(channel);
                Serial.print(" | Controller: "); Serial.print(rx.byte2);
                Serial.print(" | Value: "); Serial.println(rx.byte3);
                break;

            case 0xC0: // Program Change
                Serial.print("Program Change | ");
                Serial.print("Channel: "); Serial.print(channel);
                Serial.print(" | Program: "); Serial.println(rx.byte2);
                break;

            case 0xD0: // Channel Aftertouch (Pressure)
                Serial.print("Channel Aftertouch | ");
                Serial.print("Channel: "); Serial.print(channel);
                Serial.print(" | Pressure: "); Serial.println(rx.byte2);
                break;

            case 0xE0: // Pitch Bend
                int pitchBendValue = ((rx.byte3 << 7) | rx.byte2) - 8192;
                Serial.print("Pitch Bend | ");
                Serial.print("Channel: "); Serial.print(channel);
                Serial.print(" | Value: "); Serial.println(pitchBendValue);
                break;

            default:
                Serial.print("Unknown MIDI Message | ");
                Serial.print("Header: "); Serial.print(rx.header);
                Serial.print(" | Data1: "); Serial.print(rx.byte1);
                Serial.print(" | Data2: "); Serial.print(rx.byte2);
                Serial.print(" | Data3: "); Serial.println(rx.byte3);
                break;
        }
    }
}
