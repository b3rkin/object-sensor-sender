#include <WiFiNINA.h>
#include "src/libraries/Arduino_LSM6DSOX/src/Arduino_LSM6DSOX.h"
#include "lib/wifi.hpp"
#include "lib/led.hpp"
#include "lib/structs.hpp"


#define CMD_CHECK_INTERVAL (1000000) // The interval in microseconds in which the program checks if it has received new commands over UDP.

LedController LED;
WifiController * wifi;

PacketBuffer packetBuffer;

void setup() {

    Serial.begin(115200); // Start Serial connection if any.
    LED.init(); // Initialise the LEDs
    wifi = new WifiController; // Initialize 
    LED.set(ledColor::BLUE);
    wifi->waitUntilConnectionToAP();
    wifi->waitForFirstReceivedPacket();
    LED.clear(ledColor::BLUE);
    LED.set(ledColor::RED);
    IMU.begin();
    Wire.setClock(3000000); // Set I2C clock to max of 400kHz (Currently 3 MHz)
    delay(1000);
    IMU.calibrateSensors();
    LED.clear(ledColor::RED);
    LED.set(ledColor::GREEN);
}

void loop(){

    IMU.readSensors(packetBuffer);
    wifi->sendSensorData(packetBuffer);


}