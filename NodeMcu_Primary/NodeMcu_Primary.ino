#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>

// prototypes
boolean connectWifi();

const char* ssid     = "prettyflyforawifi";  //enter your ssid/ wi-fi(case sensitive) router name
const char* password = "mj98jaaw--";  // enter ssid password (case sensitive)

boolean wifiConnected = false;

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back


void setup()
{
  Serial.begin(115200);
  
  // Initialise wifi connection
  wifiConnected = connectWifi();

  if(wifiConnected) {
    Udp.begin(localUdpPort);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  }

  Wire.begin(5, 4); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
}


void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

    // Write to the Arduino
    Wire.beginTransmission(8);  /* begin with device address 8 */
    Wire.write(incomingPacket);  /* sends the received packet to the Arduino */
    Wire.endTransmission();    /* stop transmitting */
 
    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();
  }
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
