/*
  TODO :
  - RTP MIDI
    - update RTP MIDI to use pixi functions
    - web ui help for osc
    - save config in flash
    - load config from flash
  - pixi
    - LFOs
    - RANDOM S/H
    - flip / flop
    - timer for pixi write
    - trigger / gate detection
    - cv in
  - OSC
    - sending message to remote host
*/



#include <pgmspace.h> //PROGMEM

#include <OSCMessage.h> // dependency: https://github.com/CNMAT/OSC

//
#define OUTPUT_MODE_GATE 1
#define OUTPUT_MODE_TRIG 2
#define OUTPUT_MODE_CVUNI 3

#define OUTPUT_MODE_RANDOM_SH 20

#define OUTPUT_MODE_CVBI 50

#define OUTPUT_MODE_LFO_SQUARE 51
#define OUTPUT_MODE_LFO_SAW 52
#define OUTPUT_MODE_LFO_RAMP 53
#define OUTPUT_MODE_LFO_SINE 54

#define INPUT_MODE_GATE 100
#define INPUT_MODE_TRIG 101
#define INPUT_MODE_CVUNI 102

#define INPUT_MODE_CVBI 150



//used to build a unique named based on mac
char* myName = "oscpixi-XXXXXX";

//will enable some Serial.print
bool debug = true;


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
  delay(500);
  setupMDNS();
  setupPixi();
  createWebServerRoutes();
  startWebServer();
  setupOSCServer();
  rtpMidiSetup();

  //when to save config ?
  //saveConfiguration();

}


void loop() {
  digitalWrite(LED_BUILTIN, (millis() / 100) % 2 ? HIGH : LOW);
  receiveOSCMessage();
  handleWebClient();
  rtpMidiLoop();
  //TODO: use timer for updatePixi
  updatePixi();
}



