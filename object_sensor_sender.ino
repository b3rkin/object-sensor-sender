#include <WiFiNINA.h>
#include "src/libraries/Arduino_LSM6DSOX/src/Arduino_LSM6DSOX.h"
#include "util/util.h"

#define USB_DEBUG_MODE // This should commented if the arduino is not going to be used in USB powered debugging mode.
                       // So no-USB = comment the line. Otherwise it gets stuck.

//******************** Wifi Parameters ********************//
char ssid[] = "tracked-object-AP";        // Network SSID (name)
char pass[] = "password";                 // Network password (

// The IP address of this device generally becomes 196.168.4.1
unsigned int webServerPort = 80;     // I don't think this is needed for UDP
unsigned int localPort     = 2390;   // This is the port the main devices should listen to.
unsigned int remotePort    = 5500;   // This is the port where this arduino sends UDP Packets to.

IPAddress remoteIP;
WiFiServer server(webServerPort);
WiFiUDP Udp;

int status = WL_IDLE_STATUS;
//*********************************************************//

//******************** Sensor Parameters ********************//
packetBuffer packetBuffer;            // This is the buffer that is being sent over UDP


//***********************************************************//
void setup() {

//******************** LED Setup ********************//
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDR, HIGH);
  // digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
//***************************************************//

//******************** Serial Port Setup ********************//
  // Initialize serial and wait for port to open:
#ifdef USB_DEBUG_MODE
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect. Needed for native USB port only
#endif
//***********************************************************//

//******************** Wifi Access Point Setup ********************//
#ifdef USB_DEBUG_MODE
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  Serial.println("Access Point Web Server");

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  // Print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);
#endif

  // Create open network.
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
#ifdef USB_DEBUG_MODE
    Serial.println("Creating access point failed");
#endif
    while (true);
  }

  // Wait 5 seconds for connection:
  delay(5000);
  // Start the web server on port 80.
  server.begin();

  // Connection is established, so print out the status.
#ifdef USB_DEBUG_MODE
  printWiFiStatus();
#endif

Udp.begin(localPort); // Start a UDP server on port "localPort".
//*****************************************************************//

//******************** Wifi Connection Setup ********************//

  // Wait for the other device to connect to the access point.
  while(status !=WL_AP_CONNECTED){
    delay(200);
    status = WiFi.status();
  }

#ifdef USB_DEBUG_MODE
  Serial.println("Device connected to AP");
#endif

  // Wait for the first UDP Packet to arrive for transmission to start.
  while(1){
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      IPAddress remoteIp = Udp.remoteIP();

#ifdef USB_DEBUG_MODE
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      Serial.print(remoteIp);
      Serial.print(", port ");
      Serial.println(Udp.remotePort());
#endif

      break;
    }
  }

  // Get the remote IP and port that sended the UDP Starting message.
  remoteIP = Udp.remoteIP();
  remotePort = Udp.remotePort();
  
//*****************************************************************//



  // Initialize IMU 
  if (!IMU.begin()) {
#ifdef USB_DEBUG_MODE
    Serial.println("Failed to initialize IMU!");
#endif
    while (1);
  }

  // Set I2C clock to max of 400kHz
  Wire.setClock(400000); // Set I2C clock to max of the IMU.

  // Calibrate the sensors. Sensor should be held still and level during this period.
  delay(2000);
  IMU.calibrateSensors();


}


void loop() {




  // sendSensorData();
  // speedTestUDP();
  timeTestUDP();
}

void sendSensorData(){
  IMU.readSensors(packetBuffer);
  sendUDPPacket;
}

void speedTestUDP(){
  int packetNumber = 1000;
  int counter = 0;
  unsigned long start = micros();
  while(counter < packetNumber){
    IMU.readSensors(packetBuffer);
    sendUDPPacket();
    counter++;
  }
  unsigned long stop = micros();

#ifdef USB_DEBUG_MODE 
  Serial.println("");
  Serial.println("-------------------");
  Serial.print("It took ");
  Serial.print(stop - start);
  Serial.print(" μs");
  Serial.print(" to send ");
  Serial.print(packetNumber);
  Serial.println(" packets!");
  Serial.println("-------------------");
#endif
  while(1);
}

void timeTestUDP(){
  int counter = 0;
  int sendTime = 1000000;
  unsigned long start = micros();
  unsigned long stop = micros();
  while(stop-start <sendTime){
    IMU.readSensors(packetBuffer);
    sendUDPPacket();
    stop = micros();
    counter++;
  }
#ifdef USB_DEBUG_MODE 
  Serial.println("");
  Serial.println("-------------------");
  Serial.print(counter);
  Serial.print(" packets have been sent in ");
  Serial.print(stop - start);
  Serial.println(" μs!");
  Serial.println("-------------------");

#endif
  while(1);
}

void sendUDPPacket(){
  Udp.beginPacket(remoteIP, remotePort);
  Udp.write(packetBuffer.buffer, 28);
  Udp.endPacket();
}

void printWiFiStatus() {
  // Print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

