
// Contains EEPROM.read() and EEPROM.write()
#include <EEPROM.h>

// ID of the settings block
#define CONFIG_VERSION "cf1"

// where store configuration data in EEPROM
#define CONFIG_START 32

/**
   set to true if config has been changed and needs saving
*/
boolean configurationNeedsSave = false;

/**
   records last configuration saving, in order to avoid over-frequent saving
*/
unsigned long configurationLastSave = 0;

// Example settings structure
struct StoreStruct {
  char version[4]; // This is for mere detection if they are your settings
  boolean wifiAPMode;
  char wifiSSID[25];
  char wifiPassword[25];
  char wifiAPPassword[25];
  boolean oscEnabled;
  unsigned int oscPort;
  boolean restEnabled;
  boolean rtpMidiEnabled;
  int rtpMidiChannel;
  int channelValues[20]; // channel values (0 to 4096)
  int channelModes[20]; // channel mode
  float channelLFOFrequencies[20]; // in Hz
  float channelLFOPhases[20];  // 0 to 1
  float channelLFOPWMs[20]; // 0 to 1

} configuration = {
  CONFIG_VERSION,
  // The default values
  false, // wifi ap mode
  "redpill", // client mode ssid
  "0101010101", // client mode password
  "0101010101", // ap mode password
  true, // osc enabled
  5000, // osc port
  true, // rest
  true, // rtpMidiEnabled
  1,    // rtpMidiChannel
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // channel values
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // channel modes
  {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},//LFO frequency
  {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, //lfo phases
  {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f}, //lfo pwm
};

void loadConfiguration() {
  EEPROM.begin(1024);
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (EEPROM.read(CONFIG_START + 0) == 0 &&
      EEPROM.read(CONFIG_START + 1) == 0 &&
      EEPROM.read(CONFIG_START + 2) == 0
      || resetConfig
     ) {
    Serial.println("no configuration from EEPROM. saving defaults");
    saveConfiguration(true);
  }
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]) {
    Serial.println("loading configuration from EEPROM.");
    for (unsigned int t = 0; t < sizeof(configuration); t++) {
      *((char*)&configuration + t) = EEPROM.read(CONFIG_START + t);
    }

  } else {
    Serial.println("wrong configuration version in EEPROM.");
    saveConfiguration(true);
  }
}

void saveConfiguration(boolean force) {
  noInterrupts();
  //when to save config ?
  if (configurationNeedsSave || force) {
    if (millis() - configurationLastSave > 1000 * 60 || force) {
      configurationLastSave = millis();
      for (unsigned int t = 0; t < sizeof(configuration); t++) {
        EEPROM.write(CONFIG_START + t, *((char*)&configuration + t));
      }
      EEPROM.commit();
      Serial.println("saved configuration in EEPROM.");
    }
  }
  interrupts();
}

void printConfiguration() {
  for (unsigned int t = 0; t < sizeof(configuration); t++) {
    Serial.print(t);
    Serial.print("\t");
    Serial.println( *((char*)&configuration + t));
  }
}


