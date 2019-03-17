# ESP_HomieLED_CT

Homie Node for ESP8266 to control a colortemperature LED


## Description

This node expects two values: brightness and color temperature.
It tsets the brightness using PWM on two configured outputs. The brightness is distributed over both output based on the color temperature value.
The total power never exceeds the power of 100% duty cycle.


## Build

To be built with [platformio](https://platformio.org).

Note that this repository is configured as library, so there is no `src/main.cpp`. For testing, you can use `simple.cpp` in the example directory - just symlink or copy it to `src`.

Then, the example application can be built with platformio:

`pio run`


To adapt configuration, copy the `data/homie/config.json.example` to `data/homie/config.json` and edit it. Then run `pio run --target=uploadfs` to upload the configuration to SPIFFS of your ESP8266.    


## Dependencies

This node was developed for Homie-convention v3.0.1, so the #develop-v3 branch of [homie-esp88266](https://github.com/homieiot/homie-esp8266#develop-v3) is used.
Furthermore, it uses [HomieLoggerNode](https://github.com/euphi/HomieLoggerNode#develop-v3)  for Logging.

Dependencies are configured in `platformio.ini` / `library.json`, so they are automatically installed by platformio.
