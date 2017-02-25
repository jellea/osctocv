// https://github.com/esp8266/Arduino/blob/master/doc/libraries.md#spi

/**
   init pixi,
   set channels to their saved configuration.channelModes[].
*/
void setupPixi() {
  word Pixi_ID = pixi.config();
  // Identify shield by ID register
  if (Pixi_ID == 0x0424 ) {
    Serial.print("Found PIXI module ID: 0x");
    Serial.println(Pixi_ID, HEX);

    // set device control register
    // MAX3100 PDF page 39
    word deviceControlRegister = pixi.ReadRegister ( PIXI_DEVICE_CTRL, false);
    word adc_ctl  = 0b11;  //ADCCTL  [0:1] ADC conversion mode selection. 00:Idle mode. 01:Single sweep. 10:Single conversion one port at a time, 11:Continuous sweep This mode is not controlled by CNVT.
    word dac_ctl  = 0b00;  //DACCTL  [3:2] DAC mode selection.  00:Sequential update mode for DAC-configured ports. 01:Immediate update mode for DAC-configured ports.
    word adc_conv = 0b00;  //ADCCONV [5:4] ADC conversion rate selection. 00:200ksps (default), 01:250ksps, 10:333ksps, 11:400ksps
    word dac_ref  = 0b1;   //DACREF  [6]   DAC voltage reference 1:external, 0:internal
    /* pixi.WriteRegister(PIXI_DEVICE_CTRL, deviceControlRegister
                        | ( (adc_ctl << 14) & ADCCTL )
                        | ( (dac_ctl << 12) & DACCTL )
                        | ( (adc_conv << 10) & ADCCONV )
                        | ( (dac_ref << 9) & DACREF )
                       );*/

    pixi.WriteRegister(PIXI_DEVICE_CTRL, deviceControlRegister
                       | ADC_MODE_CONT
                       | DACREF
                       | !DACCTL
                      );

    float test = pixi.readTemperature ( TEMP_CHANNEL_INT );
    Serial.print("Internal Raw Temperature:");
    Serial.println(test);

    //init all channels
    for (int channel = 0; channel < 20; channel++) {
      channelSetModeAndValue(channel, configuration.channelModes[channel], configuration.channelValues[channel], true);
    }

  } else {
    Serial.println("No PIXI module found!");
  }
}


/**
   update pixi dac register
   read adc values and trigger events
*/
inline void updatePixi() {
  //TODO: read from ADC, detect gate and trigs
  unsigned long now = millis();
  for (int channel = 0; channel < 20; channel++) {
    //generate trigs, lfo, sh etc.
    channelProcess(channel, now);
    //write ADC values
    if (!channelIsInput(channel)) {
      pixi.WriteRegister(PIXI_DAC_DATA + channel, configuration.channelValues[channel]);
    }
    //read ADC value. we also read output channels because we use DAC monitoring.
    channelADC[channel] = pixi.ReadRegister(PIXI_ADC_DATA + channel, true);
  }
}

