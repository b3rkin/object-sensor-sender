# The Project:
This project is written in the Arduino language for the Arduino Nano RP2040 Connect. The purpose of this project is to create a wifi access point on the Arduino wifi chip and send accelerometer and gyroscope values using UDP packets to the device that connects to this access point.

## Current status of the project:
The Arduino creates a WIFI access point called "tracked-object-AP" and then starts waiting for a connection. Once a device is connected to the AP, the Arduino expects an initial UDP packet (Any random packet will do) to starts its operation. There are 3 available options/functions described below. 

## Setup

## Functions
| Function              | Purpose       |
|---------------------|-------------------------------|
|  sendSensorData() |    The device will sample the accelerometer and gyroscope and send the data in a UDP packet to the selected port continuously.              |
|    speedTestUDP()     |       The device will sample the sensors and send 1000 packets over UDP and print how long it took.          |
|  timeTestUDP()  |     The device will sample the sensors and send packets during 1 second over UDP and print how many packets have been sent.       |
| |     |
|     |          |


## Packet Format
The packet that is sent over UDP is formatted as follows:

| gyroscope X | gyroscope Y |gyroscope Z| accelerometer X | accelerometer Y | accelerometer Z| timestamp|
|-------------|-------------|-----------|-----------------|-----------------|----------------|----------|
| 4 bytes float | 4 bytes float |4 bytes float| 4 bytes float |4 bytes float | 4 bytes float|4 bytes unsigned long|


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

## Data Visualisation
Currently there are 2 python scripts available for data visualisation. One writes the raw incoming UDP packets to a file per line, the second visualises the incoming sensor data in real time. In both cases the PC needs to be connected to the Arduino via the "tracked-object-AP" access point before calling the scripts.
### save_packets_to_txt.py
This script continuously saves the received UDP packets per line in a file called "packets.txt"
### rt_sensor_visualiser.py
This script visualises the 6 incoming sensor values from the Arduino.
## TO-DO
 - [ ] Add more operating modes and configurations.
   - [ ] Add LED color scheme based on operating mode.
 - [ ] Add several commands such as start and stop that can be send over UDP to the arduino to be able to control the operation over WiFi.  
    - [ ] Add a "start" and "stop" command.
 - [ ] Add Setup info to README.
  - [ ] Add visualisation guide.
 ## References
 1. Sensor code fork of: https://github.com/arduino-libraries/Arduino_LSM6DSOX