#include "blink.h"
#include "myserial.h"

void setup() {
    //init_test_blink();
    init_serial();
}

// the loop function runs over and over again forever
void loop() {
    String mensaje;
    //test_blink();
    sendToSerial(String("Test\n"), Serial_3);
    delay(5);
    mensaje = readFromSerial(Serial_0);
    sendToSerial(mensaje, Serial_0);
    delay(500);
}