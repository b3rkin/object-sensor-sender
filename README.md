# The Project:
This project is written in the Arduino language for the Arduino Nano RP2040 Connect. The purpose of this project is to create a wifi access point on the Arduino wifi chip and send accelerometer and gyroscope values using UDP packets to the device that connects to this access point.

## Current status of the project:
The Arduino creates a WIFI access point called "tracked-object-AP" and then starts waiting for a connection. Once a device is connected to the AP, the Arduino expects an initial UDP packet (Any random packet will do) to starts its operation. There are 3 available options/functions described below. 

## Functions
| Function              | Purpose       |
|---------------------|-------------------------------|
|  sendSensorData() |    The device will sample the accelerometer and gyroscope and send the data in a UDP packet to the selected port continuously.              |
|    speedTestUDP()     |       The device will sample the sensors and send 1000 packets over UDP and print how long it took.          |
|  timeTestUDP()  |     The device will sample the sensors and send packets during 1 second over UDP and print how many packets have been sent.       |
| |     |
|     |          |

## Units
The units used are compliant with the larger project this virtual IMU is part of.
| Value               | Units used in Project         |
|---------------------|-------------------------------|
|   |                  |
|         |                    |
|    |           |
| |     |
|     |          |

## The coordinate system

## TO-DO
 - [ ] Add several commands such as start and stop that can be send over UDP to the arduino to be able to control the operation over WiFi.  
    - [ ] Add a "start" and "stop" command.

 ## References
 1. Sensor code fork of: https://github.com/arduino-libraries/Arduino_LSM6DSOX