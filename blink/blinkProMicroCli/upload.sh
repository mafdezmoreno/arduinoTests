#Arduino proMicro

# compile
arduino-cli compile --fqbn SparkFun:avr:promicro

# warning
echo "Push reset button twice to allow the upload"

# upload
arduino-cli upload --port /dev/tty.usbmodem1101 --fqbn SparkFun:avr:promicro