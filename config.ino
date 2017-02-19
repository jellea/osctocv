
// Contains EEPROM.read() and EEPROM.write()
#include <EEPROM.h>

// ID of the settings block
#define CONFIG_VERSION "cf1"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

boolean configurationNeedsSave = false;
unsigned long configurationLastSave = 0;

// Example settings structure
struct StoreStruct {
  // This is for mere detection if they are your settings
  char version[4];
  float pinValues[20];
  word pinModes[20];
  word lfoModes[20];
  word lfoPhases[20];
  boolean wifiAPMode;
  char wifiSSID[25];
  char wifiPassword[25];
  char wifiAPPassword[25];
  boolean oscEnabled;
  unsigned int oscPort;
  boolean restEnabled;
  boolean rtpMidiEnabled;
  int rtpMidiChannel;

} configuration = {
  CONFIG_VERSION,
  // The default values
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  true, //wifi ap mode
  "ssid",
  "0101010101",
  "0101010101",
  true, //osc
  5000, //osc port
  true, //rest
  true, //rtpMidiEnabled
  1,    //rtpMidiChannel
};

void loadConfiguration() {
  EEPROM.begin(512);
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (EEPROM.read(CONFIG_START + 0) == 0 &&
      EEPROM.read(CONFIG_START + 1) == 0 &&
      EEPROM.read(CONFIG_START + 2) == 0) {
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
    Serial.println("wrong configuration in eeprom.");
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


