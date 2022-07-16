#include "myserial.h"

void init_serial(){
    uint32_t timeout = 50; //50ms
    Serial.begin(speed_serial);
    Serial1.begin(speed_serial);
    Serial2.begin(speed_serial);
    Serial3.begin(speed_serial);
    Serial3.setTimeout(timeout)
}

void sendToSerial(String mensaje, serials choose_serial){

    switch (choose_serial)
    {
    case 0:
        Serial.print(mensaje);
        break;
    case 1:
        Serial1.print(mensaje);
        break;
    case 2:
        Serial2.print(mensaje);
        break;
    case 3:
        Serial3.print(mensaje);
        break;
    default:
        break;
    }
}

String readFromSerial(serials choose_serial){
    String temp = "";
    switch (choose_serial)
    {
    case 0:
        if (Serial.available())
            return Serial.readString();
        break;
    
    case 1:
        Serial1.readString();
        break;
    case 2:
        Serial2.readString();
        break;
    
    case 3:
        while (Serial3.available()>0){
            temp = temp + Serial3.readString();
        }
        return temp;
        break;
    
    default:
        break;
    }
}