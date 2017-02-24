
//modes

#define OUTPUT_MODE_GATE 1
#define OUTPUT_MODE_TRIG 2
#define OUTPUT_MODE_CVUNI 3

#define OUTPUT_MODE_FLIPFLOP 10

#define OUTPUT_MODE_RANDOM_SH 40

#define OUTPUT_MODE_CVBI 50

#define OUTPUT_MODE_LFO_SINE 71
#define OUTPUT_MODE_LFO_SAW 81
#define OUTPUT_MODE_LFO_RAMP 82
#define OUTPUT_MODE_LFO_TRI 83
#define OUTPUT_MODE_LFO_SQUARE 91

#define INPUT_MODE_GATE 100
#define INPUT_MODE_TRIG 101
#define INPUT_MODE_CVUNI 102

#define INPUT_MODE_CVBI 150

#define OUTPUT_TRIG_LOW 0 // pixi has 12bits dac
#define OUTPUT_TRIG_HIGH 4095 // pixi has 12bits dac

#define OUTPUT_TRIG_LENGTH 15 //15ms. timer is for now every ms.


/**
*   count to zero until zero to generate trig of N ms. see OUTPUT_TRIG_LENGTH
*/
word channelTrigCyles[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

word channelADC[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


inline bool channelIsInput(int channel) {
  return  configuration.channelModes[channel] >= 100;
}


inline bool channelIsLfo(int channel) {
  return configuration.channelModes[channel] > 70 && configuration.channelModes[channel] < 100;
}


inline bool channelIsBipolar(int channel) {
  int modee = configuration.channelModes[channel];
  return (modee < 50 || modee >= 100 && modee < 150) ? false : true;
}


inline int channelGetValue(int channel) {
  return configuration.channelValues[channel];
}


inline float channelGetVoltageValue(int channel) {
  if (channelIsBipolar(channel)) {
    return configuration.channelValues[channel] / 4095.0 * 10.0 - 5;
  } else {
    return configuration.channelValues[channel] / 4095.0 * 10.0;
  }
}

inline float channelGetADCVoltageValue(int channel) {
  if (channelIsBipolar(channel)) {
    return channelADC[channel] / 4095.0 * 10.0 - 5;
  } else {
    return channelADC[channel] / 4095.0 * 10.0;
  }
}


inline void channelProcess(int channel, unsigned long now) {

  //trigger mode
  if (OUTPUT_MODE_TRIG == configuration.channelModes[channel]) {
    if (channelTrigCyles[channel] > 0) {
      configuration.channelValues[channel] =  OUTPUT_TRIG_HIGH;
      channelTrigCyles[channel]--;
    } else {
      configuration.channelValues[channel] = OUTPUT_TRIG_LOW;
    }
  }

  //check if mode is LFO kind
  if (channelIsLfo(channel)) {

    float lfoPeriodInMs = (long)(1000.0 / configuration.channelLFOFrequencies[channel]);

    float lfoPhaseNow = (((long)(now + (lfoPeriodInMs * configuration.channelLFOPhases[channel]))) % (long)lfoPeriodInMs) / (float)lfoPeriodInMs;

    switch (configuration.channelModes[channel]) {
      case OUTPUT_MODE_LFO_SINE:
        configuration.channelValues[channel] = sin(lfoPhaseNow * 2 * PI) * 2048 + 2047;//TODO: use lookup table
        break;
      case OUTPUT_MODE_LFO_SAW:
        configuration.channelValues[channel] = (1 - lfoPhaseNow) * 4095;
        break;
      case OUTPUT_MODE_LFO_RAMP:
        configuration.channelValues[channel] = lfoPhaseNow * 4095;
        break;
      case OUTPUT_MODE_LFO_TRI:
        configuration.channelValues[channel] = (lfoPhaseNow < 0.5 ? lfoPhaseNow * 2 : (1 - lfoPhaseNow) * 2) * 2048 + 2047; //TODO: do me
        break;
      case OUTPUT_MODE_LFO_SQUARE:
        configuration.channelValues[channel] = (lfoPhaseNow < configuration.channelLFOPWMs[channel]) ? OUTPUT_TRIG_HIGH : OUTPUT_TRIG_LOW;
        break;
    }
  }
}



void channelSetModeAndValue(int channel, int channelMode, float value) {
  if (channel >= 0 && channel < 20) {
    //set channel mode if needed
    if (channelMode != configuration.channelModes[channel]) {
      configuration.channelModes[channel] = channelMode;
      pixi.configChannel(channel, channelIsInput(channel) ? CH_MODE_ADC_P : CH_MODE_DAC_ADC_MON, 0, channelIsBipolar(channel) ? CH_5N_TO_5P : CH_0_TO_10P, 0); //CH_MODE_DAC_ADC_MON  CH_MODE_DAC
      configurationNeedsSave = true;
      if (debug) {
        Serial.print("Channel ");
        Serial.print(" configured : ");
        if (channelIsInput(channel)) {
          Serial.print("/in/");
        } else {
          Serial.print("/out/");
        }
        Serial.print(channel + 1);
        Serial.print("/");
        Serial.println(channelGetModeName(channel));
      }
    }

    if (channelIsLfo(channel) && value > 0 && value < 1000 ) {
      //set LFO speed
      configuration.channelLFOFrequencies[channel] = value;
    } else {
      //update pixi value based on mode
       switch (channelMode) {
        case OUTPUT_MODE_GATE:
          configuration.channelValues[channel] = value > 0.5 ? OUTPUT_TRIG_HIGH : OUTPUT_TRIG_LOW;
          break;
        case OUTPUT_MODE_TRIG:
          channelTrigCyles[channel] = OUTPUT_TRIG_LENGTH;
          break;
        case OUTPUT_MODE_FLIPFLOP:
          configuration.channelValues[channel] = configuration.channelValues[channel] > OUTPUT_TRIG_LOW ? OUTPUT_TRIG_LOW :OUTPUT_TRIG_HIGH;
          break;
        case OUTPUT_MODE_CVUNI:
          configuration.channelValues[channel] = value < 0 ? 0 : value > 1 ? 4095 : (int)(value * 4095);
          break;
        case OUTPUT_MODE_CVBI:
          configuration.channelValues[channel] = value < -1 ? 0 : value > 1 ? 4095 : (int)(value * 2048 + 2047);
          break;
      }
    }

    if (debug) {
      if (channelIsInput(channel)) {
        Serial.print("/in/");
      } else {
        Serial.print("/out/");
      }
      Serial.print(channel + 1);
      Serial.print("/");
      Serial.print(channelGetModeName(channel));
      Serial.print(" ");
      Serial.print(value);
      Serial.print(" -> ");
      Serial.println(configuration.channelValues[channel]);
    }
  }
}


inline int channelParseInputMode(String modee) {
  int m = 0;
  if (modee == "gate") {
    m = INPUT_MODE_GATE;
  } else if (modee == "trig") {
    m = INPUT_MODE_TRIG;
  } else if (modee == "cvuni") {
    m = INPUT_MODE_CVUNI;
  } else if (modee == "cvbi") {
    m = INPUT_MODE_CVBI;
  }
  return m;
}


inline int channelParseOutputMode(String modee) {
  int m = 0;
  if (modee == "gate") {
    m = OUTPUT_MODE_GATE;
  } else if (modee == "trig") {
    m = OUTPUT_MODE_TRIG;
  } else if (modee == "flipflop") {
    m = OUTPUT_MODE_FLIPFLOP;
  } else if (modee == "cvuni") {
    m = OUTPUT_MODE_CVUNI;
  } else if (modee == "cvbi") {
    m = OUTPUT_MODE_CVBI;
  } else if (modee == "sh") {
    m = OUTPUT_MODE_RANDOM_SH;
  } else if (modee == "lfosine") {
    m = OUTPUT_MODE_LFO_SINE;
  } else if (modee == "lfosaw") {
    m = OUTPUT_MODE_LFO_SAW;
  } else if (modee == "lforamp") {
    m = OUTPUT_MODE_LFO_RAMP;
  } else if (modee == "lfotri") {
    m = OUTPUT_MODE_LFO_TRI;
  } else if (modee == "lfosquare") {
    m = OUTPUT_MODE_LFO_SQUARE;
  }
  return m;
}



String channelGetModeName(int channel) {
  String out = "";
  switch (configuration.channelModes[channel]) {
    case OUTPUT_MODE_GATE:
      out = "gate";
      break;
    case OUTPUT_MODE_TRIG:
      out = "trig";
      break;
    case OUTPUT_MODE_FLIPFLOP:
      out = "flipflop";
      break;
    case OUTPUT_MODE_CVUNI:
      out = "cvuni";
      break;
    case OUTPUT_MODE_CVBI:
      out = "cvbi";
      break;
    case OUTPUT_MODE_RANDOM_SH:
      out = "sh";
      break;
    case OUTPUT_MODE_LFO_SINE:
      out = "lfosine";
      break;
    case OUTPUT_MODE_LFO_SAW:
      out = "lfosaw";
      break;
    case OUTPUT_MODE_LFO_RAMP:
      out = "lforamp";
      break;
    case OUTPUT_MODE_LFO_TRI:
      out = "lfotri";
      break;
    case OUTPUT_MODE_LFO_SQUARE:
      out = "lfosquare";
      break;

    case INPUT_MODE_GATE:
      out = "gate";
      break;
    case INPUT_MODE_TRIG:
      out = "trig";
      break;
    case INPUT_MODE_CVUNI:
      out = "cvuni";
      break;
    case INPUT_MODE_CVBI:
      out = "cvbi";
      break;
  }
  return out;
}


