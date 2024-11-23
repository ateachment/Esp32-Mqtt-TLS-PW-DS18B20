# ESP32 MQTT client 
The temperature is measured with the DS18B20 digital sensor and transmitted to an MQTT server in degrees Celsius.
The format is e.g:\
19.31\
-2.5\
The transmission is TLS-encrypted and password-protected.

## Installation
Rename lib/Settings/Settings-template.h to lib/Settings/Settings.h and edit this settings file according to your requirements.\
The DS18B20 sensor is connected with the data output to PIN 4 of the AZ DevKitC_V4 board.\
Compile the VS Code PlatformIO project and transfer it to your ESP32.\
Of course, an appropriately configured MQTT broker must receive the data.
