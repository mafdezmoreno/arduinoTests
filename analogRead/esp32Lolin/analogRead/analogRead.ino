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

    Serial.println("Battery level: ");
    unsigned value = map(analogRead(pinBat), 0, 4095, 0, 450);
    Serial.println(value/100.);
    delay(500);
}