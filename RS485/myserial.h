#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <Arduino.h>

const uint32_t  speed_serial = 115200;
enum serials {Serial_0 = 0, Serial_1 = 1, Serial_2 = 2, Serial_3 = 3};
void init_serial();
void sendToSerial(String mensaje, serials choose_serial);
String readFromSerial(serials choose_serial);

#endif