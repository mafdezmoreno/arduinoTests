#ESP32 Wemos lolin
source compile.sh

# upload
echo "upload \
    --port /dev/cu.usbserial-003CE3E9 \
    -b $settings"
arduino-cli upload \
    --port /dev/cu.usbserial-003CE3E9 \
    -b $settings