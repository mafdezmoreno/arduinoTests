#include <Arduino.h>
#include <HardwareSerial.h>

int LED = 17; // The RX LED has a defined Arduino pin

void setup()
{
    pinMode(LED, OUTPUT);  // Set RX LED as an output
    Serial1.begin(115200); // Serial communication using UART
    delay(2000);
    Serial1.println("Initialized Serial Hardware UART Pins");
}

void loop()
{
    Serial1.println("Hola!"); // We will need a serial device to connect

    digitalWrite(LED, LOW); // set the RX LED ON
    delay(1000);            // wait for a second

    digitalWrite(LED, HIGH); // set the RX LED OFF
    delay(1000);             // wait for a second
}
