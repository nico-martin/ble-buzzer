# BLE Buzzer

An Arduino Sketch that provides a Bluetooth Low Energy Service whose buzzer characteristic exposes a value of "00" or "FF" depending on whether the buzzer is currently pressed or not.

It also has an LED that gives feedback about the current status.

## Circuit

![BLE Buzzer Circuit](https://uploads.nico.dev/ble-buzzer-circuit-arduino.jpg)

## BLE Services
The Arduino exposes a Bluetooth Low Energy Server with one Service:

### Buzzer Service 
UUID: `be09ae42-9157-4eed-87d1-1f3b4e31510f`  
buzzerCharacteristic: `22f64ef5-b567-4e5d-bbac-4b384e6a6c36` - BLERead | BLENotify

A one-byte characteristic that indicates whether the buzzer button is currently pressed (`0xFF`) or not (`0x00`).

## LED
The LED provides information about the status of the unit:
- Off: the unit is switched off
- Flashing (2x/second): Ready for a Bluetooth connection
- On: Connected