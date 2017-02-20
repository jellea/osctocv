#include <SPI.h>

#include <Pixi.h>  // dependency: https://github.com/wolfgangfriedrich/Pixi_demo

#define OUTPUT_TRIG_LOW 0 // pixi has 12bits dac
#define OUTPUT_TRIG_HIGH 4096 // pixi has 12bits dac

#define OUTPUT_TRIG_LENGTH 15 //15ms. timer is for now every ms.

/**
    our pixi instance !
*/
Pixi pixi;


/**
   count to zero until zero to generate trig of N ms. see OUTPUT_TRIG_LENGTH
*/
word channelTrigCyles[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


/**
   init pixi,
   set channels to their saved mode.
*/
void setupPixi() {
  word Pixi_ID = pixi.config();
  // Identify shield by ID register
  if (Pixi_ID == 0x0424 ) {
    Serial.print("Found PIXI module ID: 0x");
    Serial.println(Pixi_ID, HEX);

    //init all ports.
    for (int i = 0; i < 20; i++) {
      setChannel( i, configuration.channelModes[20], 0 );
    }

  } else {
    Serial.println("No PIXI module found!");
  }
}


/**
   set channel to a mode
   and set the value
   values should be in [0 1] (unipolar) or [-1 1] (bipolar) ranges
*/
void setChannel(int channel, int modee, float value) {
  if (channel >= 0 && channel < 20) {
    //set channel config
    if (modee != configuration.channelModes[channel]) {
      int range = (modee < 50 || modee >= 100 && modee < 150) ? CH_0_TO_10P : CH_5N_TO_5P;
      int channelMode = modee < 100 ? CH_MODE_DAC : CH_MODE_ADC_P;
      configuration.channelModes[channel] = modee;
      pixi.configChannel(channel, channelMode, 0, range, 0);
      //
      configurationNeedsSave = true;
      if (debug) {
        Serial.print("Channel ");
        Serial.print(channel + 1);
        Serial.print(" configured as ");
        switch (modee) {
          case OUTPUT_MODE_GATE:
            Serial.println("OUTPUT_MODE_GATE");
            break;
          case OUTPUT_MODE_TRIG:
            Serial.println("OUTPUT_MODE_TRIG");
            break;
          case OUTPUT_MODE_CVUNI:
            Serial.println("OUTPUT_MODE_CVUNI");
            break;
          case OUTPUT_MODE_RANDOM_SH:
            Serial.println("OUTPUT_MODE_RANDOM_SH");
            break;
          case OUTPUT_MODE_CVBI:
            Serial.println("OUTPUT_MODE_CVBI");
            break;
          case INPUT_MODE_GATE:
            Serial.println("INPUT_MODE_GATE");
            break;
          case INPUT_MODE_TRIG:
            Serial.println("INPUT_MODE_TRIG");
            break;
          case INPUT_MODE_CVUNI:
            Serial.println("INPUT_MODE_CVUNI");
            break;
          case INPUT_MODE_CVBI:
            Serial.println("INPUT_MODE_CVBI");
            break;
        }
      }
    }

    //update pixi value based on mode
    switch (modee) {
      case OUTPUT_MODE_GATE:
        configuration.channelValues[channel] = value > 0.5 ? OUTPUT_TRIG_HIGH : OUTPUT_TRIG_LOW;
        break;
      case OUTPUT_MODE_TRIG:
        channelTrigCyles[channel] = OUTPUT_TRIG_LENGTH;
        break;
      case OUTPUT_MODE_CVUNI:
        configuration.channelValues[channel] = value < 0 ? 0 : value > 1 ? 4096 : (int)(value * 4096);
        break;
      case OUTPUT_MODE_CVBI:
        configuration.channelValues[channel] = value < -1 ? 0 : value > 1 ? 4096 : (int)(value * 2048 + 2048);
        break;
      case OUTPUT_MODE_FLIPFLOP:
        configuration.channelValues[channel] = configuration.channelValues[channel] > OUTPUT_TRIG_LOW ? OUTPUT_TRIG_HIGH : OUTPUT_TRIG_LOW;
        break;
    }

    if (debug) {
      Serial.print("/");
      Serial.print(channel + 1);
      Serial.print("/");
      switch (modee) {
        case OUTPUT_MODE_GATE:
          Serial.print("OUTPUT_MODE_GATE");
          break;
        case OUTPUT_MODE_TRIG:
          Serial.print("OUTPUT_MODE_TRIG");
          break;
        case OUTPUT_MODE_CVUNI:
          Serial.print("OUTPUT_MODE_CVUNI");
          break;
        case OUTPUT_MODE_RANDOM_SH:
          Serial.print("OUTPUT_MODE_RANDOM_SH");
          break;
        case OUTPUT_MODE_CVBI:
          Serial.print("OUTPUT_MODE_CVBI");
          break;
        case INPUT_MODE_GATE:
          Serial.print("INPUT_MODE_GATE");
          break;
        case INPUT_MODE_TRIG:
          Serial.print("INPUT_MODE_TRIG");
          break;
        case INPUT_MODE_CVUNI:
          Serial.print("INPUT_MODE_CVUNI");
          break;
          break;
        case INPUT_MODE_CVBI:
          Serial.print("INPUT_MODE_CVBI");
          break;
      }
      Serial.print(" ");
      Serial.print(value);
      Serial.print(" -> ");
      Serial.println(configuration.channelValues[channel]);
    }
  }
}


/**
   update pixi dac register
   read adc values and trigger events
*/
long ccc = 0;
inline void updatePixi() {
  ccc++;
  //TODO: read from ADC, detect gate and trigs
  unsigned long now = millis();

  for (int i = 0; i < 20; i++) {

    //trigger mode
    if (OUTPUT_MODE_TRIG == configuration.channelModes[i]) {
      if (channelTrigCyles[i] > 0) {
        configuration.channelValues[i] =  OUTPUT_TRIG_HIGH;
        channelTrigCyles[i]--;
      } else {
        configuration.channelValues[i] = OUTPUT_TRIG_LOW;
      }
    }


    for (int channel = 0; i < 20; i++) {
      //check if mode is LFO kind
      if (configuration.channelModes[i] > 70 && configuration.channelModes[i] < 100) {

        float lfoPeriodInMs = (long)(1000.0 / configuration.channelLFOFrequencies[i]);

        float lfoPhaseNow = (((long)(now + (lfoPeriodInMs * configuration.channelLFOPhases[i]))) % (long)lfoPeriodInMs) / (float)lfoPeriodInMs;

        switch (configuration.channelModes[i]) {
          case OUTPUT_MODE_LFO_SINE:
            configuration.channelValues[i] = sin(lfoPhaseNow * 2 * PI) * 2048 + 2048;//TODO: use lookup table
            break;
          case OUTPUT_MODE_LFO_SAW:
            configuration.channelValues[i] = (1- lfoPhaseNow) * 4096;
            break;
          case OUTPUT_MODE_LFO_RAMP:
            configuration.channelValues[i] = lfoPhaseNow * 4096;
            break;
          case OUTPUT_MODE_LFO_TRI:
            configuration.channelValues[i] = (lfoPhaseNow > 0.5 ? lfoPhaseNow * 2 : 1 - lfoPhaseNow * 2) * 4096; //TODO: do me
            break;
          case OUTPUT_MODE_LFO_SQUARE:
            configuration.channelValues[i] = (lfoPhaseNow < configuration.channelLFOPWMs[i]) ? OUTPUT_TRIG_HIGH : OUTPUT_TRIG_LOW;
            break;
        }
        if (i == 0 && ccc%57==0) {
          //Serial.print("lfoPhaseNow:");
          //Serial.println(lfoPhaseNow);
          //Serial.print("period:");
          //Serial.println(lfoPeriodInMs);
          //Serial.print("mode:");
          //Serial.println(configuration.channelModes[i]);
          //Serial.print("value:");
          //Serial.println(configuration.channelValues[i]);
        }
      }
    }

    //actually write value to pixi
    if (configuration.channelModes[i] <  100) {
      pixi.WriteRegister(PIXI_DAC_DATA + i, configuration.channelValues[i]);
    }
  }
}


/**
   just print pixi temperature
*/
void pixiTemperature() {
  int temp = pixi.readTemperature ( TEMP_CHANNEL_INT );
  Serial.print("PIXI Internal Temperature:  ");
  Serial.println(temp);
}
