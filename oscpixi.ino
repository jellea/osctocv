// inslude the SPI library:
#include <SPI.h>
#include "Pixi.h" // dependency: https://github.com/wolfgangfriedrich/Pixi_demo
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <OSCMessage.h> // dependency: https://github.com/CNMAT/OSC
#include <OSCBundle.h>
#include <OSCData.h>

ESP8266WiFiMulti WiFiMulti;

Pixi pixi;

WiFiUDP Udp;
const unsigned int localPort = 8888;

float oscValue = 0;
OSCErrorCode error;

void setup() {
  // randomSeed(analogRead(0));
  
  word Pixi_ID = 0;
  float Temp = 0;
  word test = 0;

  Serial.begin(115200);

  WiFiMulti.addAP("", "");
  
  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while(WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());

  Serial.println("");
  Serial.println("Searching PIXI module");

  Pixi_ID = pixi.config();

  // Identify shield by ID register       
  if (Pixi_ID == 0x0424 ) {
    Serial.print("Found PIXI module ID: 0x");
    Serial.println(Pixi_ID,HEX);

    Temp = pixi.readTemperature ( TEMP_CHANNEL_INT );
    Serial.print("Internal Temperature:  ");
    Serial.println(Temp);

    // Set all ports to DAC and 0-10V
    for (int i = 0; i < 20; i++) {
      pixi.configChannel ( i, CH_MODE_DAC, 0, CH_0_TO_10P, 0 );
    }
    
  }
  else{
    Serial.println("No PIXI module found!");
    Serial.println();
  }
}

// borrowed from https://github.com/CNMAT/OSC/blob/master/examples/UDPReceive/UDPReceive.ino#L29
char * numToOSCAddress( int pin){
    static char s[10];
    int i = 9;
  
    s[i--]= '\0';
  do
    {
    s[i] = "0123456789"[pin % 10];
                --i;
                pin /= 10;
    }
    while(pin && i);
    s[i] = '/';
    return &s[i];
}

void setDac(OSCMessage &msg, int addrOffset) {
  oscValue = msg.getFloat(0);

  for (int i = 0; i < 20; i++) {
    int outMatch = msg.match(numToOSCAddress(i), addrOffset);
    
    if (outMatch){

      pixi.writeAnalog(i, oscValue * 409.6 );
    }
  }
}

//void setDac(OSCMessage &msg, int addrOffset) {
//  oscValue = msg.getFloat(0);
////  Serial.println("OSC received! ");
////  Serial.println(oscValue);
//
//  for (int i = 0; i < 20; i++) {
//    int outMatch = msg.match(numToOSCAddress(i), addrOffset);
//    if (outMatch){
//      pixi.configChannel ( i, CH_MODE_DAC, 0, CH_0_TO_10P, 0 );
//    }
//  }
//
//
//
//}

void loop() {
  OSCBundle bundle;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      bundle.fill(Udp.read());
    }
    if (!bundle.hasError()) {
      bundle.route("/vout", setDac);
      bundle.route("/pswitch", setDac);
  }}
  delay(1);
  
//  Stuff for testing  
//  
//  // ADC loop to output a positive ramp on channel 0
//
//  for (word value = 0; value < 4096; value++) { 
//    pixi.writeAnalog ( CHANNEL_0, value );
//    delay(1);
//  }
//
//  // Random on all 20 channels
//
//  for (int i = 0; i < 20; i++) {
//    pixi.writeAnalog (i, random(4000) );
//  }
//  delay(5);
}
