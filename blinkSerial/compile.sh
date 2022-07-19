#Compile using arduino-cli

#Arduino Uno
#arduino-cli compile --fqbn arduino:avr:one blinkSerial.ino

#Arduino proMicro
# compile
arduino-cli compile --fqbn SparkFun:avr:promicro
# upload
arduino-cli upload --port /dev/tty.usbmodem1101 --fqbn SparkFun:avr:promicro