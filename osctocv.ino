/*
  TODO :
  - general
    - simpler mode mgmt
  - RTP MIDI
    - default mappings
    - web and readme help
    - 14 bits midi cc support
    - save config in flash
    - load config from flash
    - mdns discovery
  - pixi
    - LFOs
    - tempo synced lfo
    - trigger / gate detection
    - cv in ->
  - OSC
    - sending message to remote host
*/

#include <pgmspace.h> //PROGMEM
#include <OSCMessage.h> // dependency: https://github.com/CNMAT/OSC
#include <Ticker.h>
#include <Pixi.h>  // dependency: https://github.com/wolfgangfriedrich/Pixi_demo

/**
    our pixi instance !
*/
Pixi pixi;


/**
 * timer for updating pixi
 * todo look at faster timers ?
 */
Ticker ticker;

// reset config in eeprom
boolean resetConfig = false;

//used to build a unique named based on mac address
char* myName = "oscpixi-xxxx";

//will enable some Serial.print
bool debug = true;

unsigned long lastTimer = 0;


void setup() {

  //basic stuff
  Serial.begin(115200);
  Serial.print("\n\n");
  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);

  //unique name
  setupUniqueName();
  Serial.print(myName);
  Serial.println(" starting.");

  loadConfiguration();

  setupWifi();
  //delay(500);
  setupMDNS();
  setupPixi();
  createWebServerRoutes();
  startWebServer();
  setupOSCServer();
  setupRtpMidi();
  setupTimer();
}


void inline onTimer() {
  unsigned long now = millis();
  if (lastTimer != 0 && now - lastTimer > 2){
    Serial.print("timer underrun. last run was ");
    Serial.print(now - lastTimer);
    Serial.println("ms ago.");
  }
  lastTimer = now;
  //
  updatePixi();
}

void setupTimer() {
  ticker.attach_ms(1, onTimer);
}


void loop() {
  //blink me please
  digitalWrite(LED_BUILTIN, (millis() / 100) % 2 ? HIGH : LOW);
  //handle clients
  receiveOSCMessage();
  handleWebClient();
  rtpMidiLoop();
  //save configuration if needed
  saveConfiguration(false);
}















