#include <SPI.h>   // Required for RF library.
#include "RF24.h"  // Library for nRF24L01.

// Connect to L298H.
const int RWhPWMPin = 9;   // Right motor PWM speed control.
const int RWhFwdPin = 14;  // Right motor Direction 1.
const int RWhBwdPin = 4;   // Right motor Direction 2.
const int LWhFwdPin = 7;   // Left motor Direction 1.
const int LWhBwdPin = 6;   // Left motor Direction 2.
const int LWhPWMPin = 5;   // Left motor PWM speed control.

// Declare integer array for car.
int data[4];

// 8 = CE --- 10 = CSN.
RF24 radio(8, 10);

// This is the address that will receive data from the transmitter.
const uint64_t pipe = 0xE8E8F0F0E1LL;


void setup() {
  pinMode(RWhFwdPin, OUTPUT);
  pinMode(LWhFwdPin, OUTPUT);
  pinMode(RWhBwdPin, OUTPUT);
  pinMode(LWhBwdPin, OUTPUT);
  Serial.begin(9600);
  radio.begin();                   // Activate connection.
  radio.openReadingPipe(1, pipe);  // Determine the address that will receive data.
  radio.startListening();          // Enable receiving.
}

void loop() {
  if (radio.available()) {
    // Read data from transmitter.
    radio.read(data, sizeof(data));

    // Joystick data X:
    if (data[0] > 550) {
      digitalWrite(RWhFwdPin, HIGH);
      digitalWrite(RWhBwdPin, LOW);
      digitalWrite(LWhFwdPin, HIGH);
      digitalWrite(LWhBwdPin, LOW);

      // Map joystick position to PWM speed.
      data[2] = map(data[0], 550, 1023, 0, 255);
      data[3] = map(data[0], 550, 1023, 0, 255);

      // Drive motors.
      analogWrite(RWhPWMPin, data[2]);
      analogWrite(LWhPWMPin, data[3]);
      Serial.println("GO FORWARD");
    }

    // Joystick data X:
    if (data[0] < 400) {
      digitalWrite(RWhFwdPin, LOW);
      digitalWrite(RWhBwdPin, HIGH);
      digitalWrite(LWhFwdPin, LOW);
      digitalWrite(LWhBwdPin, HIGH);

      // Map joystick position to PWM speed.
      data[2] = map(data[0], 400, 0, 0, 255);
      data[3] = map(data[0], 400, 0, 0, 255);

      // Drive motors.
      analogWrite(RWhPWMPin, data[2]);
      analogWrite(LWhPWMPin, data[3]);
      Serial.println("GO BACKWARD");
    }

    // Joystick data Y:
    if (data[1] > 550) {
      digitalWrite(RWhFwdPin, LOW);
      digitalWrite(RWhBwdPin, HIGH);
      digitalWrite(LWhFwdPin, HIGH);
      digitalWrite(LWhBwdPin, LOW);

      // Map joystick position to PWM speed.
      data[2] = map(data[1], 550, 1023, 0, 255);
      data[3] = map(data[1], 550, 1023, 0, 255);

      // Drive motors.
      analogWrite(RWhPWMPin, data[2]);
      analogWrite(LWhPWMPin, data[3]);
      Serial.println("TURN RIGHT");
    }

    // Joystick data Y:
    if (data[1] < 400) {
      digitalWrite(RWhFwdPin, HIGH);
      digitalWrite(RWhBwdPin, LOW);
      digitalWrite(LWhFwdPin, LOW);
      digitalWrite(LWhBwdPin, HIGH);

      // Map joystick position to PWM speed.
      data[2] = map(data[1], 1023, 400, 0, 255);
      data[3] = map(data[1], 1023, 400, 0, 255);

      // Drive motors.
      analogWrite(RWhPWMPin, data[2]);
      analogWrite(LWhPWMPin, data[3]);
      Serial.println("TURN LEFT");
    }

    // Check if joystick is centered:
    if (data[0] < 550 && data[0] > 400 && data[1] < 550 && data[1] > 400) {
      digitalWrite(RWhFwdPin, LOW);
      digitalWrite(RWhBwdPin, LOW);
      digitalWrite(LWhFwdPin, LOW);
      digitalWrite(LWhBwdPin, LOW);
      data[0] = 0;
      data[1] = 0;
      data[2] = 0;
      data[3] = 0;
      Serial.println("STOP");
    }

    // Prevent "buzzing" at very low speeds.
    if (data[2] < 8) data[0] = 0;
    if (data[3] < 8) data[1] = 0;
  }
}