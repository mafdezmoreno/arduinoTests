#include <Arduino.h>

void setup() {
	Serial.begin(115200);
	Serial.setTimeout(200); //The default value is 1000ms
	delay(2000); //Time to setup the serial before first print
	Serial.print(0x22, HEX);
	Serial.flush(); //To wait until sending completion 
}

void loop() {
	bool reception = false;
	String command;
	char newChar;

	if(Serial.available()) // Data in buffer?
	{
		//command = Serial.readStringUntil('\n'); //! Works OK
		//command = Serial.readString(); //! Works OK

		newChar = Serial.read(); //In difference to previous command, this only captures the first character of the input buffer. For example, from the print inside "setup" now we only capture "2"
		reception = true;
	}
	delay(10);
	if (reception)
	{
		//Serial.print(" ECHO: " + command);
		//Serial.print(newChar); //Prints "2"
		Serial.print(newChar, HEX); //Print "32" = 2 in hexadecimal
		Serial.flush(); //! Important. To wait serial print and avoid block the serial port
		reception = false;
	}
}
