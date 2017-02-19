
// https://github.com/esp8266/Arduino/tree/master/doc/esp8266wifi
#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>

ESP8266WiFiMulti WiFiMulti;


void setupWifi() {
  
  if (!configuration.wifiAPMode) {
    //wifi client
    WiFiMulti.addAP(configuration.wifiSSID, configuration.wifiPassword);
    Serial.print("Wait for WiFi");
    int t = 500;
    int elapsed = 0;
    while (WiFiMulti.run() != WL_CONNECTED && elapsed < 5 * 1000) {
      Serial.print(".");
      delay(t);
      elapsed += t;
    }
    if (WiFiMulti.run() != WL_CONNECTED) {
      Serial.print("\nFailed to connect to ");
      Serial.print(configuration.wifiSSID);
      Serial.println(".");
      Serial.println("Falling back to AP mode.");
      setupWifiAP();
    } else {
      Serial.print("\nConnected to ");
      Serial.print(configuration.wifiSSID);
      Serial.print(" with IP ");
      Serial.println(WiFi.localIP());

    }

  } else {
    setupWifiAP();
  }
}

void setupWifiAP() {
  Serial.print("Wifi in AP Mode SSID:");
  Serial.print(myName);
  Serial.print(" password:");
  Serial.print(configuration.wifiAPPassword);
  boolean result = WiFi.softAP(myName, configuration.wifiAPPassword);
  if (result == true) {
    Serial.print(" IP address: ");
    Serial.println(WiFi.softAPIP() );
  } else {
    Serial.println(": Failed!");
  }
}

void setupMDNS() {

  if (!MDNS.begin(myName)) {
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started");
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("osc", "udp", 5000);//http://opensoundcontrol.org/files/Rendezvous-OSC.pdf "_osc._udp."
    MDNS.addService("midi", "udp", 5004);
  }
}


void setupUniqueName() {
  //unique name
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String n = "oscpixi-" + macID;
  //char AP_NameChar[n.length() + 1];
  memset(myName, 0, n.length() + 1);
  for (int i = 0; i < n.length(); i++)
    myName[i] = n.charAt(i);
   WiFi.hostname(myName);
}
