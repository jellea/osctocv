#include "AppleMidi.h" //https://github.com/lathoub/Arduino-AppleMidi-Library

bool rtpMidiIsConnected = false;

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h


void setupRtpMidi() {
  if (configuration.rtpMidiEnabled) {
    Serial.println("RTPMidi on port 5004");

    AppleMIDI.begin(myName);

    AppleMIDI.OnConnected(rtpMidiConnected);
    AppleMIDI.OnDisconnected(rtpMidiDisconnected);

    //AppleMIDI.OnReceiveNoteOn(rtpMidiOnNoteOn);
    //AppleMIDI.OnReceiveNoteOff(rtpMidiOnNoteOff);
    AppleMIDI.OnReceiveControlChange(rtpMidiOnControlChange);
  }
}


/**
 * handle incoming messages 
 * and protocol management tasks
 */
void rtpMidiLoop() {
  // Listen for incoming messages
  if (configuration.rtpMidiEnabled) {
    AppleMIDI.run();
  }
}


void rtpMidiConnected(uint32_t ssrc, char* name) {
  rtpMidiIsConnected  = true;
}


void rtpMidiDisconnected(uint32_t ssrc) {
  rtpMidiIsConnected  = false;
}


void rtpMidiOnNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("RTPMIDI : note on :");
  Serial.print(channel);
  Serial.print(" ");
  Serial.print(note);
  Serial.print(" ");
  Serial.println(velocity);
}


void rtpMidiOnNoteOff(byte channel, byte note, byte velocity) {
  Serial.print("RTPMIDI : note off :");
  Serial.print(channel);
  Serial.print(" ");
  Serial.print(note);
  Serial.print(" ");
  Serial.println(velocity);
}


void rtpMidiOnControlChange(byte midiChannel, byte controller, byte midiValue) {
  if (midiChannel == configuration.rtpMidiChannel || midiChannel == configuration.rtpMidiChannel + 1){
      int modeIndex = controller%10;
      int channel = controller/10 + (midiChannel == configuration.rtpMidiChannel + 1)?10:0;
      float value = midiValue/127.0;
      int modee = 0;
      switch(modeIndex){
           case 0: modee=OUTPUT_MODE_GATE;break;
           case 1: modee=OUTPUT_MODE_TRIG;break;
           case 2: modee=OUTPUT_MODE_CVUNI;break;
           case 3: modee=OUTPUT_MODE_CVBI;break;
           case 4: modee=OUTPUT_MODE_RANDOM_SH;break;
           case 5: modee=OUTPUT_MODE_LFO_SINE;break;
           case 6: modee=OUTPUT_MODE_LFO_SAW;break;
           case 7: modee=OUTPUT_MODE_LFO_RAMP;break;
           case 8: modee=OUTPUT_MODE_LFO_TRI;break;
           case 9: modee=OUTPUT_MODE_LFO_SQUARE;break;
       }
       channelSetModeAndValue(channel, modee, value);
  }
  /*Serial.print("RTPMIDI : cc :");
  Serial.print(channel);
  Serial.print(" ");
  Serial.print(controller);
  Serial.print(" ");
  Serial.println(value);*/
}


void rtpMidiNoteOn(byte channel, byte note, byte velocity) {
  AppleMIDI.noteOn(note, velocity, channel);
}


void rtpMidiNoteOff(byte channel, byte note, byte velocity) {
  AppleMIDI.noteOff(note, velocity, channel);
}


void rtpMidiControlChange(byte channel, byte cc, byte value) {
  AppleMIDI.controlChange(cc, value, channel);
}
