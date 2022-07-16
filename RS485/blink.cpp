#include "blink.h"


void init_test_blink()
{
    pinMode(led_out, OUTPUT);
}
      

void test_blink()
{
    digitalWrite(led_out, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(pause);                       // wait for a second
    digitalWrite(led_out, LOW);    // turn the LED off by making the voltage LOW
    delay(pause);   
}
