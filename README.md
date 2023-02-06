# Seplos BMS WiFi Bridge

The project targets to be a WiFi bridge for the Seplos BMS. It will collect telemetry data from the battery and push it to InfluxDB. Communication with the BMS is done over the RS485 interface.

It will also provide mechanism to heat the battery by switched 48V directly from the battery terminals.

Supports only single battery. Will likely not work if the batteries are connected in master/slave mode.

## Building the project

The project uses a common set of tools that are availabe in another repo - https://github.com/funnybrum/esp8266-base. Clone the esp8266-base repo in the lib folder:

```
cd lib
git clone git@github.com:funnybrum/esp8266-base.git
```

## Updating the firmware
The firmware support OTA (Over the Air Updates). Execute a post request against the http://{address}/update device API with attached file named "image". Or shortly use the following command as example:

```
curl -F "image=@.pio/build/d1_mini/firmware.bin" http://{address}/update
```