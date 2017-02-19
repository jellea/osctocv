/*
  TODO :
  - RTP MIDI
    - default mappings
    - web and readme help
    - 14 bits midi cc support
    - save config in flash
    - load config from flash
    - mdns discovery
  - pixi
    - LFOs
    - RANDOM S/H
    - flip / flop
    - trigger / gate detection
    - cv in ->
  - OSC
    - sending message to remote host
*/

#include <pgmspace.h> //PROGMEM

#include <OSCMessage.h> // dependency: https://github.com/CNMAT/OSC
#include <Ticker.h>

Ticker ticker;


//
#define OUTPUT_MODE_GATE 1
#define OUTPUT_MODE_TRIG 2
#define OUTPUT_MODE_CVUNI 3

#define OUTPUT_MODE_RANDOM_SH 20

#define OUTPUT_MODE_CVBI 50

#define OUTPUT_MODE_LFO_SQUARE 51
#define OUTPUT_MODE_LFO_SAW 52
#define OUTPUT_MODE_LFO_RAMP 53
#define OUTPUT_MODE_LFO_TRI 55
#define OUTPUT_MODE_LFO_SINE 56

#define INPUT_MODE_GATE 100
#define INPUT_MODE_TRIG 101
#define INPUT_MODE_CVUNI 102

#define INPUT_MODE_CVBI 150


//used to build a unique named based on mac
char* myName = "oscpixi-XXXXXX";

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
  digitalWrite(LED_BUILTIN, (millis() / 100) % 2 ? HIGH : LOW);
  receiveOSCMessage();
  handleWebClient();
  rtpMidiLoop();
  
  saveConfiguration(false);
}















