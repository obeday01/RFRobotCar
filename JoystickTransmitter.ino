#include <SPI.h>   // Required for RF library.
#include "RF24.h"  // Library for nRF24L01.

// Declare and initialize the analog pins.
int joystickX = A0;
int joystickY = A1;

// Declare variable values.
int dataX;
int dataY;

// Declare integer array to be sent.
int data[2];

// 8 = CE --- 10 = CSN.
RF24 radio(8, 10);

// This is the address that will transmit data to the receiver.
const uint64_t pipe = 0xE8E8F0F0E1LL;


void setup(void) {
  Serial.begin(9600);
  radio.begin();                // Activate connection.
  radio.openWritingPipe(pipe);  // Determine the address that will send data.
}

void loop() {
  // Initialize variable values using analog data.
  dataX = analogRead(joystickX);
  dataY = analogRead(joystickY);

  // dataX in first index and dataY in second index.
  data[0] = dataX;
  data[1] = dataY;

  // Print to see if correct data is being sent.
  Serial.print("Data X:");
  Serial.println(dataX);
  Serial.print("Data Y:");
  Serial.println(dataY);

  // Send the data.
  radio.write(data, sizeof(data));
}