#include "MIDIUSB.h"
#define NUM_BUTTONS  1

const uint8_t button1 = 2;
const uint8_t ledPin = 13;

const uint8_t buttons[NUM_BUTTONS] = {button1};
const byte controlCommands[NUM_BUTTONS] = {1};

uint8_t pressedButtons = 0x00;
uint8_t previousButtons = 0x00;


void setup() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i], INPUT);
  }
  
  pinMode(ledPin, OUTPUT);
}

void loop() {
  readButtons();
  sendCommand();
}

void readButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (digitalRead(buttons[i]) == HIGH)
    {
      bitWrite(pressedButtons, i, 1);
      delay(50);
    }
    else {
      bitWrite(pressedButtons, i, 0);
    }
  }
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void sendCommand() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (bitRead(pressedButtons, i) != bitRead(previousButtons, i)) {
      if (bitRead(pressedButtons, i)) {
        bitWrite(previousButtons, i , 1);
        controlChange(0, controlCommands[i], 0);
        digitalWrite(ledPin, HIGH);
        MidiUSB.flush();
      } else {
        bitWrite(previousButtons, i , 0);
        digitalWrite(ledPin, LOW);
      }
    }
  }
}
