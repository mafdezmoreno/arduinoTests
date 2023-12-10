#ESP32 Wemos lolin

settings="esp32:esp32:lolin32:EraseFlash=all,PartitionScheme=default,CPUFreq=10,FlashFreq=80,DebugLevel=verbose"

echo "Setting to compile:"
echo "   $settings"
echo

echo "Listing all settings (defaults + new configuration)"
# esp32:esp32:lolin32
arduino-cli board details \
    -b $settings \

# compile
arduino-cli compile \
    -b $settings \
    --build-path build \
    .