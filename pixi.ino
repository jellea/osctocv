#include <SPI.h>


/**
 *  init pixi,
 *  set channels to their saved mode.
*/
void setupPixi() {
  word Pixi_ID = pixi.config();
  // Identify shield by ID register
  if (Pixi_ID == 0x0424 ) {
    Serial.print("Found PIXI module ID: 0x");
    Serial.println(Pixi_ID, HEX);

    //init all ports.
    for (int i = 0; i < 20; i++) {
      channelSetModeAndValue(i, configuration.channelModes[20], 0);
    }

  } else {
    Serial.println("No PIXI module found!");
  }
}


/**
 * update pixi dac register
 * read adc values and trigger events
*/
inline void updatePixi() {
  //TODO: read from ADC, detect gate and trigs
  unsigned long now = millis();
  for (int channel = 0; channel < 20; channel++) {
    //generate trigs, lfo, sh etc.
    channelProcess(channel, now);
    //actually write value to pixi
    pixi.writeAnalog(channel, configuration.channelValues[channel]);
    if (!channelIsInput(channel)) {
      //pixi.WriteRegister(PIXI_DAC_DATA + channel, configuration.channelValues[channel]);
    }
    channelADC[channel] = pixi.ReadRegister(PIXI_ADC_DATA + channel, true);
  }
}

