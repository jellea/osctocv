
// https://github.com/esp8266/Arduino/tree/master/doc/esp8266wifi
#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino

#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>

ESP8266WiFiMulti WiFiMulti;

const char* wifiSSID = "redpill";
const char* wifiPassword = "0101010101";

const char* wifiAPPassword = "0101010101";

boolean wifiAPMode = false;


void setupWifi() {
  if (wifiAPMode) {
    Serial.print("Wifi in AP Mode");
    //TODO: unique name : https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/example-sketch-ap-web-server
    boolean result = WiFi.softAP(myName, wifiAPPassword);
    if (result == true) {
      Serial.print(", IP address: ");
      Serial.println(WiFi.softAPIP() );
    } else {
      Serial.println(": Failed!");
    }


  } else {
    //wifi client
    WiFiMulti.addAP(wifiSSID, wifiPassword);
    Serial.print("Wait for WiFi");
    while (WiFiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(250);
    }
    Serial.print("\nConnected to ");
    Serial.print(wifiSSID);
    Serial.print(" with IP ");
    Serial.println(WiFi.localIP());

  }
}

void setupMDNS(){
 
    if (!MDNS.begin("oscpixi")) {
      Serial.println("Error setting up MDNS responder!");
    } else {
      Serial.println("mDNS responder started");
      MDNS.addService("http", "tcp", 80);
      MDNS.addService("osc", "udp", 5000);
      MDNS.addService("rtpmidi", "udp", 5004);
    }  
}


void setupUniqueName(){
    //unique name
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String n = "oscpixi-" + macID;
   //char AP_NameChar[n.length() + 1];
  memset(myName, 0, n.length() + 1);
  for (int i=0; i<n.length(); i++)
    myName[i] = n.charAt(i);
 }
