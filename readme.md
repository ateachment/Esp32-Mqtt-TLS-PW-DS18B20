# ESP32 MQTT client 
The temperature is measured with the DS18B20 digital temperature sensor and transmitted to an MQTT server in degrees Celsius.
The format is e.g:\
19.31\
-2.5\
The transmission is TLS-encrypted and password-protected.

## Installation
Rename lib/Settings/Settings-template.h to lib/Settings/Settings.h and edit this settings file according to your requirements.\
The DS18B20 sensor is connected with the data output to PIN 4 of the AZ DevKitC_V4 board.\
Compile the VS Code PlatformIO project and transfer it to your ESP32.\
Of course, an appropriately configured MQTT broker must receive the data.

## Notes
### Using the IO pins of the ESP32
https://github.com/T-vK/ESP32-BLE-Keyboard/issues/245\
...\
Please ensure you don't connect these pins while programming GPIO0, GPIO2, GPIO5, 
GPIO12, GPIO14, and GPIO15.\
If you don't connect anything to those pins check RX, TX signal.\
If you need to connect to those pins check these conditions:\
GPIO 0 (must be LOW to enter boot mode)\
GPIO 2 (must be floating or LOW during boot)\
GPIO 5 (must be HIGH during boot)\
GPIO 12 (must be LOW during boot)\
GPIO 15 (must be HIGH during boot)\
...
