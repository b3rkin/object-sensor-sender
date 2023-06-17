#include <WiFiNINA.h>
// #include <WiFiUdp.h>

#define USB_DEBUG_MODE // This should commented if the arduino is not going to be used in USB powered debugging mode.
                       // So no-USB = comment the line. Otherwise it gets stuck.

char ssid[] = "tracked-object-AP";        // your network SSID (name)
char pass[] = "password";                 // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                         // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// The IP address of this device generally becomes 196.168.4.1
unsigned int webServerPort = 80;    // This has nothing to do with UDP
unsigned int localPort     = 2390;      // This is the port the main devices should listen to
unsigned int remotePort    = 5500;     // This is the port where this arduino sends UDP Packets to.
IPAddress remoteIP;
WiFiServer server(webServerPort);
WiFiUDP Udp;

uint8_t packetBuffer[28];           // This is the buffer that is being send over UDP

void setup() {

//******************** LED Setup ********************//
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
  // digitalWrite(LEDR, HIGH);
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
  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);
#endif

  // Create open network.
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    // Serial.println("Creating access point failed");
    while (true);
  }

  // wait 5 seconds for connection:
  delay(5000);
  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
#ifdef USB_DEBUG_MODE
  printWiFiStatus();
#endif
  Udp.begin(localPort);

  // Wait for the computer to connect to the access point.
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
  // Get the remote IP and port that sended the UDP Starting message
  remoteIP = Udp.remoteIP();
  remotePort = Udp.remotePort();
  
  // Insert dummy values in the packetBuffer.
  for(int i = 0; i < 28; i ++){
 
    packetBuffer[i] = (byte) 0x07;
  }

}

#define NUMBER_TEST

void loop() {


#ifdef NUMBER_TEST
  int counter = 0;
  unsigned long start = micros();
  while(counter < 1000){
    sendUDPPacket();
    counter++;
  }
  unsigned long stop = micros();

  while(1);
#endif
#ifndef NUMBER_TEST
  int counter = 0;
  unsigned long start = micros();
  unsigned long stop = micros();
  while(stop-start <1000){
    sendUDPPacket();
    stop = micros();
    counter++;
  }
  // Serial.print("Time: ");
  // Serial.println(stop - start);
  // Serial.print("Counter: ");
  // Serial.println(counter);
  while(1);
#endif

}

void sendUDPPacket(){
  Udp.beginPacket(remoteIP, remotePort);
  Udp.write(packetBuffer, 28);
  Udp.endPacket();
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

