
#include <WiFiUdp.h>
#include <OSCBundle.h>
#include <OSCData.h>


WiFiUDP Udp;


/**
 *  setup osc server on configured port
 */
void setupOSCServer() {
  if (configuration.oscEnabled) {
    Udp.begin(configuration.oscPort);
    Serial.print("OSC on port ");
    Serial.println(Udp.localPort());
  }
}


/**
 *  handle osc messages on the wire
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
 *  "/out/" message handling
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

  if (msg.fullMatch("gate", addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseOutputMode("trig"), value);
  } else if (msg.fullMatch("cvuni", addrOffset) || msg.fullMatch("cv", addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseOutputMode("cvuni"), value);
  } else if (msg.fullMatch("cvbi" - 1, addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseOutputMode("cvbi"), value);
  } else if (msg.fullMatch("flipflop", addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseOutputMode("flipflop"), value);
  } else if (msg.match("lfosine", addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseOutputMode("lfosine"), value);
  } else if (msg.match("lfosaw", addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseOutputMode("lfosaw"), value);
  } else if (msg.match("lforamp", addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseOutputMode("lforamp"), value);
  } else if (msg.match("lfotri", addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseOutputMode("lfotri"), value);
  } else if (msg.match("lfosquare", addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseOutputMode("lfosquare"), value);
  }
}


/**
 *  "/in/" messages handling
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
    channelSetModeAndValue(channel - 1, channelParseInputMode("trig"), 1);
  } else if (msg.match("gate", addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseInputMode("gate"), 0);
  } else if (msg.match("cvuni", addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseInputMode("cbuni"), 0);
  } else if (msg.match("cvbi", addrOffset)) {
    channelSetModeAndValue(channel - 1, channelParseInputMode("cvbi"), 0);
  }
}


/**
 * get channel from osc path at currect addrOffset
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


/**
 * get channel number as char*
 * borrowed from https://github.com/CNMAT/OSC/blob/master/examples/UDPReceive/UDPReceive.ino#L29
 */
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

