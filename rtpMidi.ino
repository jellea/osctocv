#include "AppleMidi.h" //https://github.com/lathoub/Arduino-AppleMidi-Library

bool rtpMidiIsConnected = false;

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

void rtpMidiSetup() {
  if (configuration.rtpMidiEnabled) {
  Serial.println("RTPMidi on port 5004");
  
  AppleMIDI.begin(myName);

  AppleMIDI.OnConnected(rtpMidiConnected);
  AppleMIDI.OnDisconnected(rtpMidiDisconnected);
  
  AppleMIDI.OnReceiveNoteOn(rtpMidiOnNoteOn);
  AppleMIDI.OnReceiveNoteOff(rtpMidiOnNoteOff);
  AppleMIDI.OnReceiveControlChange(rtpMidiOnControlChange);
  }
}


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

void rtpMidiOnControlChange(byte channel, byte controller, byte value) {
  Serial.print("RTPMIDI : cc :");
  Serial.print(channel);
  Serial.print(" ");
  Serial.print(controller);
  Serial.print(" ");
  Serial.println(value);
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
