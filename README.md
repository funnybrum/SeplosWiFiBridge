# Seplos BMS WiFi Bridge

## Summary
The project targets to be a WiFi bridge for the Seplos BMS. It will collect telemetry data from the battery and push it to InfluxDB. Communication with the BMS is done over the RS485 interface.

It will also provide mechanism to heat the battery by switched 48V directly from the battery terminals.

Supports only single battery. Will likely not work if the batteries are connected in master/slave mode.

## Current state
This is still work in progress. The heating is implemented, but the cooling and active balancer enablement are not yet implemented. These should be pretty straight forward - set specific pin in HIGH or LOW state when certain conditions are met.


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

## Hardware
The [Seplos WiFi Bridge PCB](https://oshwlab.com/funnybrum/wifi-to-rs485-bridge_copy) project is opensource too. I'm using JLC PCB for making the PCBs. All components are put by me, but I suppose JLC PCB can do that for you if you don't want to spend the time in soldering.

The device is powered directly from the 48V on the battery. The HLK-PM01 5V power supply works fine with 48V DC input despite being designed for an AC input.
