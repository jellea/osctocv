#include <SPI.h>

#include <Pixi.h>  // dependency: https://github.com/wolfgangfriedrich/Pixi_demo

#define OUTPUT_TRIG_LOW 0
#define OUTPUT_TRIG_HIGH 4096

//length of the Trigs. will depend on pixi write timer.
#define OUTPUT_TRIG_LENGTH 50

Pixi pixi;

// value for the pin [0-4096]
float pinValues[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//pin mode. see OUTPUT_ and INPUT_ constants
word pinModes[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//LFO
word lfoModes[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
word lfoPhases[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//count to zero until zero to generate trig of N ms. see OUTPUT_TRIG_LENGTH
word pinTrigCyles[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setupPixi() {
  word Pixi_ID = pixi.config();
  // Identify shield by ID register
  if (Pixi_ID == 0x0424 ) {
    Serial.print("Found PIXI module ID: 0x");
    Serial.println(Pixi_ID, HEX);

    //init all ports. 
    for (int i = 0; i < 20; i++) {
      setChannel( i, pinModes[20], 0 );
    }

  } else {
    Serial.println("No PIXI module found!");
  }
}



void setChannel(int channel, int modee, float value) {
  //set channel config
  if (modee != pinModes[channel]) {
    int range = (modee < 50 || modee >= 100 && modee < 150) ? CH_0_TO_10P : CH_5N_TO_5P;
    int channelMode = modee < 100 ? CH_MODE_DAC : CH_MODE_ADC_P;
    pinModes[channel] = modee;
    pixi.configChannel(channel, channelMode, 0, range, 0);

    if (debug) {
      Serial.print("Channel ");
      Serial.print(channel);
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
          break;
        case INPUT_MODE_CVBI:
          Serial.println("INPUT_MODE_CVBI");
          break;
      }
    }
  }

  //update pixi value based on mode
  switch (modee) {
    case  OUTPUT_MODE_GATE:
      pinValues[channel] = value > 0.5 ? OUTPUT_TRIG_HIGH : OUTPUT_TRIG_LOW;
      break;
    case  OUTPUT_MODE_TRIG:
      pinTrigCyles[channel] = OUTPUT_TRIG_LENGTH;
      break;
    case  OUTPUT_MODE_CVUNI:
      pinValues[channel] = value < 0 ? 0 : value > 1 ? 4096 : value * 4096;
      break;
    case  OUTPUT_MODE_CVBI:
      pinValues[channel] = value < -1 ? 0 : value > 1 ? 4096 : value * 2048 + 2048;
      break;
  }

  
  if (debug) {
    Serial.print("/");
    Serial.print(channel);
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
    Serial.println(pinValues[channel]);
  }

}


//update pixi dac register
//TODO: read from ADC, detect gate and trigs
inline void updatePixi() {

  for (int i = 0; i < 20; i++) {

    //trigger mode
    if (OUTPUT_MODE_TRIG == pinModes[i]) {
      if (pinTrigCyles[i] > 0) {
        pinValues[i] =  OUTPUT_TRIG_HIGH;
        pinTrigCyles[i]--;
      } else {
        pinValues[i] = OUTPUT_TRIG_LOW;
      }
    }

    if (pinModes[i] <  100) {
      //pixi.writeAnalog (i, pinValues[i]);
      pixi.WriteRegister( PIXI_DAC_DATA + i, pinValues[i] );
      //}else{
      //    pinValues[i] = pixi.readAnalog(i) > 0;
    }
  }
}

void pixiTemperature() {
  int temp = pixi.readTemperature ( TEMP_CHANNEL_INT );
  Serial.print("PIXI Internal Temperature:  ");
  Serial.println(temp);
}
