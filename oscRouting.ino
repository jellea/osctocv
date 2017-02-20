
#include <WiFiUdp.h>

#include <OSCBundle.h>
#include <OSCData.h>
WiFiUDP Udp;

/**
   setup osc server on configured port
*/
void setupOSCServer() {
  if (configuration.oscEnabled) {
    Udp.begin(configuration.oscPort);
    Serial.print("OSC on port ");
    Serial.println(Udp.localPort());
  }
}


/**
   handle osc messages on the wire
*/
void receiveOSCMessage() {
  if (configuration.oscEnabled) {
    OSCErrorCode error;
    OSCBundle bundle;
    OSCMessage message;
    int size = Udp.parsePacket();
    bool first = true;
    bool isMessage = false;
    if (size > 0) {
      while (size--) {
        uint8_t i = Udp.read();
        //detect if this is a bundle or a single OSC message
        //for some reason OSCBundle is not able to parse single messages
        if (first && i == '/') {
          isMessage = true;
        }
        first = false;
        if (isMessage) {
          message.fill(i);
        } else {
          bundle.fill(i);
        }
      }
      if (isMessage) {
        message.route("/out", oscOutMsg);
        message.route("/in", oscInMsg);
      } else {
        if (!bundle.hasError()) {
          bundle.route("/out", oscOutMsg);
          bundle.route("/in", oscInMsg);
        } else {
          error = bundle.getError();
          Serial.print("error in osc bundle: ");
          Serial.println(error);
        }
      }
    }
  }
}

/**
   "/out/" message handling
*/
void oscOutMsg(OSCMessage & msg, int addrOffset) {

  //get channel
  int channel = getChannel(msg, addrOffset);
  if (channel == -1) {
    return;
  }

  //address offset for channel
  addrOffset++;
  addrOffset++;
  addrOffset++;
  if (channel > 9) {
    addrOffset++;
  }

  float value = 0;
  if (msg.isFloat(0)) {
    value = msg.getFloat(0);
  } else if (msg.isDouble(0)) {
    value = msg.getDouble(0);
  } else if (msg.isInt(0)) {
    value = msg.getInt(0);
  }

  if (msg.fullMatch("trig", addrOffset)) {
    if (value > 0) {
      setChannel(channel, OUTPUT_MODE_TRIG, 1);
    }
  } else if (msg.fullMatch("gate", addrOffset)) {
    setChannel(channel, OUTPUT_MODE_GATE, value);
  } else if (msg.fullMatch("cvuni", addrOffset) || msg.fullMatch("cv", addrOffset)) {
    setChannel(channel, OUTPUT_MODE_CVUNI, value);
  } else if (msg.fullMatch("cvbi", addrOffset)) {
    setChannel(channel, OUTPUT_MODE_CVBI, value);
  } else if (msg.fullMatch("flipflop", addrOffset)) {
    setChannel(channel, OUTPUT_MODE_FLIPFLOP, value);
  }
}


/**
   "/in" messages handling
*/
void oscInMsg(OSCMessage & msg, int addrOffset) {
  //get channel
  int channel = getChannel(msg, addrOffset);
  if (channel == -1) {
    return;
  }

  //address offset for channel
  addrOffset++;
  addrOffset++;
  addrOffset++;
  if (channel > 9) {
    addrOffset++;
  }

  float value = msg.getFloat(0);
  if (msg.match("trig", addrOffset)) {
    setChannel(channel, OUTPUT_MODE_TRIG, 1);
  } else if (msg.match("gate", addrOffset)) {
    setChannel(channel, INPUT_MODE_GATE, 0);
  } else if (msg.match("cvuni", addrOffset)) {
    setChannel(channel, INPUT_MODE_CVUNI, 0);
  } else if (msg.match("cvbi", addrOffset)) {
    setChannel(channel, INPUT_MODE_CVBI, 0);
  } else if (msg.match("lfo/sine", addrOffset)) {
    setChannel(channel, OUTPUT_MODE_LFO_SINE, 0);
    configuration.channelLFOFrequencies[channel] = value;
  } else if (msg.match("lfo/saw", addrOffset)) {
    setChannel(channel, OUTPUT_MODE_LFO_SAW, 0);
    configuration.channelLFOFrequencies[channel] = value;
  } else if (msg.match("lfo/ramp", addrOffset)) {
    setChannel(channel, OUTPUT_MODE_LFO_RAMP, 0);
    configuration.channelLFOFrequencies[channel] = value;
  } else if (msg.match("lfo/tri", addrOffset)) {
    setChannel(channel, OUTPUT_MODE_LFO_TRI, 0);
    configuration.channelLFOFrequencies[channel] = value;
  } else if (msg.match("lfo/square", addrOffset)) {
    setChannel(channel, OUTPUT_MODE_LFO_SQUARE, 0);
    configuration.channelLFOFrequencies[channel] = value;    
  }
}

/**
   get channel from osc path at currect addrOffset
*/
int getChannel(OSCMessage & msg, int addrOffset) {
  int channel = -1;
  for (int i = 0; i < 20; i++) {
    if (msg.match(numToOSCAddress(i), addrOffset) > 0) {
      channel = i;
      break;
    }
  }
  return channel;
}


// borrowed from https://github.com/CNMAT/OSC/blob/master/examples/UDPReceive/UDPReceive.ino#L29
char * numToOSCAddress(int channel) {
  static char s[10];
  int i = 9;
  s[i--] = '\0';
  do {
    s[i] = "0123456789"[channel % 10];
    --i;
    channel /= 10;
  } while (channel && i);
  s[i] = '/';
  return &s[i];
}

