#include <Arduino.h>

// Pin connected to battery
// (use a 10/5k resistor as protection)
const int pinBat = 34;

// If it is expected a voltage below 3.3V use a voltage divider to
// don't broke the input pin.

void setup() {
    Serial.begin(115200);
    delay(1000);
}

void loop() {

    Serial.println(analogRead(pinBat));
    delay(500);
}