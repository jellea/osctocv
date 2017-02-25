
// https://github.com/esp8266/Arduino/blob/master/doc/libraries.md#eeprom
#include <EEPROM.h>

// ID of the settings block
#define CONFIG_VERSION "cf3"

// where store configuration data in EEPROM
#define CONFIG_START 32

/**
 * set to true if config has been changed and needs saving
 */
boolean configurationNeedsSave = false;

/**
 * records last configuration saving, in order to avoid over-frequent saving
 */
unsigned long configurationLastSave = 0;

/**
 * configuration struc
 */
struct StoreStruct {
  char version[4]; // This is for mere detection if they are your settings
  boolean debug;
  boolean wifiAPMode;
  char wifiSSID[25];
  char wifiPassword[25];
  char wifiAPPassword[25];
  boolean oscEnabled;
  unsigned int oscPort;
  boolean restEnabled;
  boolean rtpMidiEnabled;
  int rtpMidiChannel;
  word adcVoltageReference;
  word nbAvgADCSamples;
  int channelValues[20]; // channel values (0 to 4095)
  int channelModes[20]; // channel mode
  float channelLFOFrequencies[20]; // in Hz
  float channelLFOPhases[20];  // 0 to 1
  float channelLFOPWMs[20]; // 0 to 1
} configuration = {
  CONFIG_VERSION,
  true,  // debug mode
  false,  // wifi ap mode
  "redpill", // client mode ssid
  "0101010101", // client mode password
  "0101010101", // ap mode password
  true, // osc enabled
  5000, // osc port
  true, // rest
  true, // rtpMidiEnabled
  1,    // rtpMidiChannel
  0x0b,    // adcVoltageReference 0 internal, 1 external
  0x1b,    // number of adc samples. in power of two.
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // channel values
  {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}, // channel modes
  {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},// LFO frequency
  {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, // LFO phases (interesting to save to keep the relative phases)
  {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f}, // LFO pwm
};


/**
 *  ESP8266 epprom is not working 100% the same to std arduino one.
 *  https://github.com/esp8266/Arduino/blob/master/doc/libraries.md#eeprom
 */
void setupConfiguration() {
  EEPROM.begin(512);
}


/**
 *  load configuration from eeprom.
 */
void loadConfiguration() {
  // To make sure there are settings, and they are ours!
  // If nothing is found it will use the default settings.
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]
      && !resetConfig) {
    Serial.println("loading configuration from EEPROM.");
    for (unsigned int t = 0; t < sizeof(configuration); t++) {
      *((char*)&configuration + t) = EEPROM.read(CONFIG_START + t);
    }
  } else {
    if (EEPROM.read(CONFIG_START + 0) == 0 &&
        EEPROM.read(CONFIG_START + 1) == 0 &&
        EEPROM.read(CONFIG_START + 2) == 0
       ) {
      Serial.println("no configuration from EEPROM. saving default config in EEPROM.");
    } else {
      Serial.println("wrong configuration version in EEPROM. saving default config in EEPROM.");
    }
    saveConfiguration(true);
  }
}


/**
 * save config in eeprom
 */
void saveConfiguration(boolean force) {
  //noInterrupts();
  //when to save config ?
  if (configurationNeedsSave || force) {
    if (millis() - configurationLastSave > 1000 * 60 || force) {
      configurationLastSave = millis();
      configurationNeedsSave = false;
      for (unsigned int t = 0; t < sizeof(configuration); t++) {
        EEPROM.write(CONFIG_START + t, *((char*)&configuration + t));
      }
      EEPROM.commit();
      Serial.println("saved configuration in EEPROM.");
    }
  }
  //interrupts();
}


/**
 * dump configuration on Serial
 */
void printConfiguration() {
  Serial.print("Configuration dump (size: ");
  Serial.print(sizeof(configuration));
  Serial.print(")");
  for (unsigned int t = 0; t < sizeof(configuration); t++) {
    if (t % 50 == 0) {
      Serial.println(" ");
    }
    Serial.print( *((char*)&configuration + t) );
    Serial.print(" ");
  }
}


