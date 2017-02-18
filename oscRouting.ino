
#include <WiFiUdp.h>

#include <OSCBundle.h>
#include <OSCData.h>
WiFiUDP Udp;

const unsigned int oscPort = 5000;


void setupUDPServer() {
  Udp.begin(oscPort);
  Serial.print("OSC on port ");
  Serial.println(Udp.localPort());
}


void receiveOSCMessage() {
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

void oscOutMsg(OSCMessage & msg, int addrOffset) {

  //get index
  int index = indexOfPin(msg, addrOffset);
  if (index == -1) {
    return;
  }

  //address offset for index
  addrOffset++;
  addrOffset++;
  addrOffset++;
  if (index > 9) {
    addrOffset++;
  }

  //if (debug) {
  //  Serial.print("/out/");
  //  Serial.print(index);
  //}
  float value = 0;
  if (msg.isFloat(0)) {
    value = msg.getFloat(0);
  } else if (msg.isDouble(0)) {
    value = msg.getDouble(0);
  } else if (msg.isInt(0)) {
    value = msg.getInt(0);
  }

  if (msg.fullMatch("trig", addrOffset)) {
    //if (debug) Serial.print("/trig ");
    if (value > 0) {
      setChannel(index, OUTPUT_MODE_TRIG, 1);
    }
  } else if (msg.fullMatch("gate", addrOffset)) {
    //if (debug) Serial.print("/gate ");
    setChannel(index, INPUT_MODE_GATE, value);
  } else if (msg.fullMatch("cvuni", addrOffset) || msg.fullMatch("cv", addrOffset)) {
    //if (debug)  Serial.print("/cvuni ");
    setChannel(index, INPUT_MODE_CVUNI, value);
  } else if (msg.fullMatch("cvbi", addrOffset)) {
    //if (debug)  Serial.print("/cvbi ");
    setChannel(index, INPUT_MODE_CVBI, value);
  }
  //if (debug) Serial.println(value);
}

void oscInMsg(OSCMessage & msg, int addrOffset) {
  //get index
  int index = indexOfPin(msg, addrOffset);
  if (index == -1) {
    return;
  }

  //address offset for index
  addrOffset++;
  addrOffset++;
  addrOffset++;
  if (index > 9) {
    addrOffset++;
  }

  float value = msg.getFloat(0);
  if (msg.match("trig", addrOffset)) {
    setChannel(index, OUTPUT_MODE_TRIG, 1);
  } else if (msg.match("gate", addrOffset)) {
    setChannel(index, INPUT_MODE_GATE, 0);
  } else if (msg.match("cvuni", addrOffset)) {
    setChannel(index, INPUT_MODE_CVUNI, 0);
  } else if (msg.match("cvbi", addrOffset)) {
    setChannel(index, INPUT_MODE_CVBI, 0);
  }
}


int indexOfPin(OSCMessage & msg, int addrOffset) {
  int index = -1;
  for (int i = 0; i < 20; i++) {
    if (msg.match(numToOSCAddress(i), addrOffset) > 0) {
      index = i;
      break;
    }
  }
  return index;
}


// borrowed from https://github.com/CNMAT/OSC/blob/master/examples/UDPReceive/UDPReceive.ino#L29
char * numToOSCAddress(int pin) {
  static char s[10];
  int i = 9;
  s[i--] = '\0';
  do {
    s[i] = "0123456789"[pin % 10];
    --i;
    pin /= 10;
  } while (pin && i);
  s[i] = '/';
  return &s[i];
}

