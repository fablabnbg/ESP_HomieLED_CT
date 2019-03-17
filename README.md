# ESP_HomieLED_CT

Homie Node for ESP8266 to control a colortemperature LED


## Description

This node expects two values: brightness and color temperature.
It tsets the brightness using PWM on two configured outputs. The brightness is distributed over both output based on the color temperature value.
The total power never exceeds the power of 100% duty cycle.


## Build

To be build with [platformio](platformio.org).

Note that this repository is configured as library, so there is no `src/main.cpp`. For testing, you can use the `simple.cpp` in the example directory - just symlink or copy it to `src`.

Then, the example configuration can be built with platformio:

`pio run`


To adapt configuration, copy the `data/homie/config.json.example` to `data/homie/config.json` and edit it. Then run `pio run --target=uploadfs` to upload the configuration to SPIFFS of your ESP8266.    
