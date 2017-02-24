#include <ESP8266WebServer.h>

ESP8266WebServer webServer(80);

// pages are stored in progmem/flash to save RAM
const char homeHTML[] PROGMEM = "<html>\n<title>OSCPixi</title>\n<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' crossorigin='anonymous'>\n<div class='container' style='width:450px;margin20px;padding:20px'>\n \n  <nav class='navbar navbar-default'>\n   <div class='container-fluid'>\n     <div class='collapse navbar-collapse'>\n        <ul class='nav navbar-nav'>\n       <li class='active'><a href='/'>OSCPixi</a><li>\n        <li><a href='/rest'>Rest API</a></li>\n       <li><a href='/wifi'>WLAN</a></li>\n       <li><a href='/status'>Status</a></li>\n       </ul>\n     </div>\n    </div>\n  </nav>\n  \n  <h1>OSCPixi</h1>\n  <p>OSCPixi is. what it is.</p>\n  \n  <h2>OSC API</h2>\n  <p>port 5000<p>\n <p>/out/&lt;1-20&gt;/&lt;trig|gate|cvuni|cvbi|lfo/sine|lfo/saw|lfo/ramp|lfo/tri|lfo/square&gt; float</p>\n  <p>/in/&lt;1-20&gt;/&lt;trig|gate|cvuni|cvbi&gt;</p>\n  \n  <h2>RTP MIDI</h2>\n <p>todo</p>\n \n  <h2>Rest Api</h2>\n <p>/out/?channel=&lt;1-20&gt;&amp;mode=&lt;trig|gate|cvuni|cvbi&gt;&amp;value=float</p>\n <p>/in/?channel&lt;1-20&gt;&amp;mode=&lt;trig|gate|cvuni|cvbi&gt;</p>\n <p> see this web <a href='/rest'>form</a></p>\n \n  <h2>Source</h2>\n <p> \n    source available on \n    <a href='https://github.com/hdavid/osctocv'>https://github.com/hdavid/osctocv</a>\n </p>\n  \n  <h2>Updates</h2>\n  <p> \n    via usb. make sure to unplug the module from your modular power supply first !\n  </p>\n</div>\n<html>";
const char restHTML[] PROGMEM = "<html>\n<title>OSCPixi</title>\n<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' crossorigin='anonymous'>\n<div class='container' style='width:450px;margin20px;padding:20px'>\n\n <nav class='navbar navbar-default'>\n   <div class='container-fluid'>\n     <div class='collapse navbar-collapse'>\n        <ul class='nav navbar-nav'>\n       <li><a href='/'>OSCPixi</a><li>\n       <li class='active'><a href='/rest'>Rest API</a></li>\n        <li><a href='/wifi'>WLAN</a></li>\n       <li><a href='/status'>Status</a></li>\n       </ul>\n     </div>\n    </div>\n  </nav>  \n  \n  <h2>outputs</h2>\n  <form action='/out' target='resultoutput'>\n    <div class='form-group'>\n      channel<br/>\n      <input type='radio' name='channel' id='co1' value='1'/>&nbsp;<label for='co1'>1</label>\n     <input type='radio' name='channel' id='co2' value='2'/>&nbsp;<label for='co2'>2</label>\n     <input type='radio' name='channel' id='co3' value='3'/>&nbsp;<label for='co3'>3</label>\n     <input type='radio' name='channel' id='co4' value='4'/>&nbsp;<label for='co4'>4</label>\n     <input type='radio' name='channel' id='co5' value='5'/>&nbsp;<label for='co5'>5</label>\n     <input type='radio' name='channel' id='co6' value='6'/>&nbsp;<label for='co6'>6</label>\n     <input type='radio' name='channel' id='co7' value='7'/>&nbsp;<label for='co7'>7</label>\n     <input type='radio' name='channel' id='co8' value='8'/>&nbsp;<label for='co8'>8</label>\n     <input type='radio' name='channel' id='co9' value='9'/>&nbsp;<label for='co9'>9</label>\n     <input type='radio' name='channel' id='co10' value='10'/>&nbsp;<label for='co10'>10</label>\n     <input type='radio' name='channel' id='co11' value='11'/>&nbsp;<label for='co12'>11</label>\n     <input type='radio' name='channel' id='co12' value='12'/>&nbsp;<label for='co12'>12</label>\n     <input type='radio' name='channel' id='co13' value='13'/>&nbsp;<label for='co13'>13</label>\n     <input type='radio' name='channel' id='co14' value='14'/>&nbsp;<label for='co14'>14</label>\n     <input type='radio' name='channel' id='co15' value='15'/>&nbsp;<label for='co15'>15</label>\n     <input type='radio' name='channel' id='co16' value='16'/>&nbsp;<label for='co16'>16</label>\n     <input type='radio' name='channel' id='co17' value='17'/>&nbsp;<label for='co17'>17</label>\n     <input type='radio' name='channel' id='co18' value='18'/>&nbsp;<label for='co18'>18</label>\n     <input type='radio' name='channel' id='co19' value='19'/>&nbsp;<label for='co19'>19</label>\n     <input type='radio' name='channel' id='co20' value='20'/>&nbsp;<label for='co20'>20</label>\n   </div>\n    <div class='form-group'>\n      mode<br/>\n     <input type='radio' name='mode' id='mo1' value='trig'/>&nbsp;<label for='mo1'>trigger</label>\n     <input type='radio' name='mode' id='mo2' value='gate'/>&nbsp;<label for='mo2'>gate</label>\n      <input type='radio' name='mode' id='mo22' value='flipflop'/>&nbsp;<label for='mo22'>flipflop</label>\n      <input type='radio' name='mode' id='mo3' value='cvuni'/>&nbsp;<label for='mo3'>unipolar cv (0v +10v)</label>\n      <input type='radio' name='mode' id='mo4' value='cvbi'/>&nbsp;<label for='mo4'>biploar cv (-5v +5v)</label>\n      <input type='radio' name='mode' id='mo5' value='lfosine'/>&nbsp;<label for='mo5'>sine&nbsp;lfo</label>\n      <input type='radio' name='mode' id='mo6' value='lfosaw'/>&nbsp;<label for='mo6'>saw&nbsp;lfo</label>\n      <input type='radio' name='mode' id='mo7' value='lforamp'/>&nbsp;<label for='mo7'>ramp&nbsp;lfo</label>\n      <input type='radio' name='mode' id='mo8' value='lfotri'/>&nbsp;<label for='mo8'>triangle&nbsp;lfo</label>\n     <input type='radio' name='mode' id='mo9' value='lfosquare'/>&nbsp;<label for='mo9'>square&nbsp;lfo</label>\n    </div>\n    <div class='form-group'>\n      <label for='value'>value</label>\n      <input name='value' id='value' class='form-control' placeholder='[0, 1] for unipolar, [-1, 1] for bipolar, frequency (Hz) for LFOs'/>\n   </div>\n    <input type='submit'/>\n  </form>\n <iframe src='' name='resultoutput' style='border:none;width:250px;height:40px;'></iframe>\n\n <h2>inputs</h2>\n <form action='/in' target='resultinput'>\n    <div class='form-group'>\n      channel<br/>\n      <input type='radio' name='channel' id='ci1' value='1'/>&nbsp;<label for='ci1'>1</label>\n     <input type='radio' name='channel' id='ci2' value='2'/>&nbsp;<label for='ci2'>2</label>\n     <input type='radio' name='channel' id='ci3' value='3'/>&nbsp;<label for='ci3'>3</label>\n     <input type='radio' name='channel' id='ci4' value='4'/>&nbsp;<label for='ci4'>4</label>\n     <input type='radio' name='channel' id='ci5' value='5'/>&nbsp;<label for='ci5'>5</label>\n     <input type='radio' name='channel' id='ci6' value='6'/>&nbsp;<label for='ci6'>6</label>\n     <input type='radio' name='channel' id='ci7' value='7'/>&nbsp;<label for='ci7'>7</label>\n     <input type='radio' name='channel' id='ci8' value='8'/>&nbsp;<label for='ci8'>8</label>\n     <input type='radio' name='channel' id='ci9' value='9'/>&nbsp;<label for='ci9'>9</label>\n     <input type='radio' name='channel' id='ci10' value='10'/>&nbsp;<label for='ci10'>10</label>\n     <input type='radio' name='channel' id='ci11' value='11'/>&nbsp;<label for='ci12'>11</label>\n     <input type='radio' name='channel' id='ci12' value='12'/>&nbsp;<label for='ci12'>12</label>\n     <input type='radio' name='channel' id='ci13' value='13'/>&nbsp;<label for='ci13'>13</label>\n     <input type='radio' name='channel' id='ci14' value='14'/>&nbsp;<label for='ci14'>14</label>\n     <input type='radio' name='channel' id='ci15' value='15'/>&nbsp;<label for='ci15'>15</label>\n     <input type='radio' name='channel' id='ci16' value='16'/>&nbsp;<label for='ci16'>16</label>\n     <input type='radio' name='channel' id='ci17' value='17'/>&nbsp;<label for='ci17'>17</label>\n     <input type='radio' name='channel' id='ci18' value='18'/>&nbsp;<label for='ci18'>18</label>\n     <input type='radio' name='channel' id='ci19' value='19'/>&nbsp;<label for='ci19'>19</label>\n     <input type='radio' name='channel' id='ci20' value='20'/>&nbsp;<label for='ci20'>20</label>\n   </div>\n    <div class='form-group'>\n      mode<br/>\n     <input type='radio' name='mode' id='mi1' value='trig'/>&nbsp;<label for='mi1'>trigger</label>\n     <input type='radio' name='mode' id='mi2' value='gate'/>&nbsp;<label for='mi2'>gate</label>\n      <input type='radio' name='mode' id='mi3' value='cvuni'/>&nbsp;<label for='mi3'>unipolar cv (0v +10v)</label>\n      <input type='radio' name='mode' id='mi4' value='cvbi'/>&nbsp;<label for='mi4'>biploar cv (-5v +5v)</label>\n    </div>\n    <div class='form-group'>\n      <label for='value'>host</label>\n     <input name='host' id='host' class='form-control' placeholder='hostname or ip address'/>\n      <input name='port' id='port' class='form-control' placeholder='port (udp)'/>\n    </div>\n    <input type='submit'/>\n  </form>\n <iframe src='' name='resultinput' style='border:none;width:250px;height:40px;'></iframe>\n\n</div>\n<html>";
const char wifiHTML[] PROGMEM = "<html>\n<title>OSCPixi</title>\n<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' crossorigin='anonymous'>\n<div class='container' style='width:450px;margin20px;padding:20px'>\n \n  <nav class='navbar navbar-default'>\n   <div class='container-fluid'>\n     <div class='collapse navbar-collapse'>\n        <ul class='nav navbar-nav'>\n       <li><a href='/'>OSCPixi</a><li>\n       <li><a href='/rest'>Rest API</a></li>\n       <li class='active'><a href='/wifi'>WLAN</a></li>\n        <li><a href='/status'>Status</a></li>\n       </ul>\n     </div>\n    </div>\n  </nav>\n  \n  <form action='/wifi/config' target='wifioutput'>\n    <div class='form-group'>\n      <input type='radio' name='mode' id='mode1' value='client'/>&nbsp;<label for='mode1'>client</label>\n      <input type='radio' name='mode' id='mode2' value='pa'/>&nbsp;<label for='mode2'>access-point</label>\n    </div>\n    <div class='form-group'>\n      <label for='ssid'>ssid (client)</label>\n     <input name='mode' id='ssid'  class='form-control' placeholder='your router ssid'/>\n   </div>\n    <div class='form-group'>\n      <label for='password'>password (client)</label>\n     <input name='password' id='password'  class='form-control' placeholder='your router wifi password'/>\n    </div>\n    <input type='submit'/>\n  </form>\n \n  <iframe src='' name='wifioutput' style='border:none;width:250px;height:40px;'></iframe>\n \n  <p>\n   in Access Point mode you will find your device as 'oscpixi-XX' under the list of Wifi networks. \n    the wifi password in AP mode is 0101010101\n  </p>\n  <p>\n   If the device cannot connect to the specified access point as client, \n    it will fallback to access point mode.\n  </p>\n  \n</div>\n<html>";
const char statusHTML[] PROGMEM = "<html>\n<title>OSCPixi</title>\n<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' crossorigin='anonymous'>\n<div class='container' style='width:450px;margin20px;padding:20px'>\n \n  <nav class='navbar navbar-default'>\n   <div class='container-fluid'>\n     <div class='collapse navbar-collapse'>\n        <ul class='nav navbar-nav'>\n       <li><a href='/'>OSCPixi</a><li>\n       <li><a href='/rest'>Rest API</a></li>\n       <li><a href='/wifi'>WLAN</a></li>\n       <li class='active'><a href='/status'>Status</a></li>\n        </ul>\n     </div>\n    </div>\n  </nav>\n\n  \n  <iframe src='/status/data' name='' style='border:none;width:450px;height:400px;'></iframe>\n  \n  \n</div>\n<html>";

//String constants stored in progmem/flash to save RAM
const char textHMTL[] PROGMEM = "text/html";
const char valueMustBeZeroOne[] PROGMEM = "value must be between 0 and 1";
const char valueMustBeMinusOneOne[] PROGMEM = "value must be between -1 and 1";
const char outputModeError[] PROGMEM = "mode must be one of trig, gate, cvuni, cvbi";
const char inputModeError[] PROGMEM = "mode must be one of trig, gate, cvuni, cvbi";
const char indexError[] PROGMEM = "index must be between 1 and 20";
const char okReboot[] PROGMEM = "ok. power cycle your device.";

/**
   handle web traffic
*/
void handleWebClient() {
  if (configuration.restEnabled) {
    webServer.handleClient();
  }
}


/**
   start our web server
*/
void startWebServer() {
  if (configuration.restEnabled) {
    Serial.println("Rest API on port 80");
    webServer.begin();
  }
}


/**
   create routes that our webserver must use.
*/
void createWebServerRoutes() {

  if (configuration.restEnabled) {

    webServer.onNotFound(webServerSendHome);
    
    webServer.on("/", []() {
      webServer.send_P(200, textHMTL, homeHTML);
    });
    
    webServer.on("/rest", []() {
      webServer.send_P(200, textHMTL, restHTML);
    });
    
    webServer.on("/wifi", []() {
      webServer.send_P(200, textHMTL, wifiHTML);
    });
    
    webServer.on("/status", []() {
      webServer.send_P(200, textHMTL, statusHTML);
    });
    
    webServer.on("/blank", []() {
      webServer.send_P(200, textHMTL, "");
    });
    
    webServer.on("/status/data", []() {
      String string = "<html><link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' crossorigin='anonymous'><table style='width:400px'>";
      string += "<tr><td>channel</td><td>mode</td><td>in</td><td>volts</td><td>out</td><td>volts</td></tr>";
      for (int channel = 0; channel < 20; channel++) {
        string += "<tr>";
        string += "<td>";
        string += (channel + 1);
        string += "</td><td>";
        if (channelIsInput(channel)) {
          string += "/in/";
        } else {
          string += "/out/";
        }
        string += (channel + 1);
        string += "/";
        string += channelGetModeName(channel);
        string += " </td><td>";
        string += configuration.channelValues[channel];
        string += " </td><td>";
        string += channelGetVoltageValue(channel);
        string += "v</td><td>";
        string += channelADC[channel];
        string += " </td><td>";
        string += channelGetADCVoltageValue(channel);
        string += "v</td>";
        if (channelIsLfo(channel)) {
          string += "<td>";
          string += configuration.channelLFOFrequencies[channel];
          string += "Hz</td>";
          string += " </td>";
        }
        string += "</tr>\n";
      }
      string += "</table></html>";
      webServer.send(200, "text/html", string);
    });


    // output endpoint
    webServer.on("/out", []() {
      String modee =  webServer.arg("mode");
      int channel = webServer.arg("channel").toInt();
      float value = webServer.arg("value").toFloat();

      if (channel > 0 && channel <= 20) {
        if (modee == "trig" || modee == "gate" || modee == "cvuni" || modee == "flipflop") {
          if (value > 1 || value < 0) {
            webServer.send_P(400, textHMTL, valueMustBeMinusOneOne);
          } else {
            channelSetModeAndValue(channel - 1, channelParseOutputMode(modee), value);
            webServerSendOK();
          }
        } else  if (modee == "cvbi") {
          if (value > 1 || value < -1) {
            webServer.send_P(400, textHMTL, valueMustBeMinusOneOne);
          } else {
            channelSetModeAndValue(channel - 1, channelParseOutputMode(modee), value);
            webServerSendOK();
          }
        } else  if (modee == "lfosine" || modee == "lfosaw" || modee == "lforamp" || modee == "lfotri" || modee == "lfosquare") {
          if (value <= 0  || value > 1000 ) {
            webServer.send_P(400, textHMTL, "lfo value must be above 0 and below 1000");
          } else {
            channelSetModeAndValue(channel - 1, channelParseOutputMode(modee), value);
            webServerSendOK();
          }
        } else {
          webServer.send_P(400, textHMTL, outputModeError);
        }
      } else {
        webServer.send_P(400, textHMTL, indexError);
      }
    });


    //inputs endpoint
    webServer.on("/in", []() {
      String modee =  webServer.arg("mode");
      int channel = webServer.arg("channel").toInt();

      if (channel > 0 && channel <= 20) {
        if (modee == "trig" || modee == "gate" || modee == "cvuni" || modee == "flipflop") {
          channelSetModeAndValue(channel - 1, channelParseInputMode(modee), 0);
          webServerSendOK();
        } else  if (modee == "cvbi") {
          channelSetModeAndValue(channel - 1, channelParseInputMode(modee), 0);
          webServerSendOK();
        } else {
          webServer.send_P(400, textHMTL, inputModeError);
        }
      } else {
        webServer.send_P(400, textHMTL, indexError);
      }
    });


    //wifi config endpoint
    webServer.on("/wifi/config", []() {
      String modee =  webServer.arg("mode");
      String ssid =  webServer.arg("ssid");
      String password =  webServer.arg("login");
      if (modee == "ap" || modee  == "client") {
        for (int i = 0; i < ssid.length(); i++) {
          configuration.wifiSSID[i] = ssid.charAt(i);
          configuration.wifiSSID[i + 1] = 0;//ssid.charAt(i);
        }
        for (int i = 0; i < password.length(); i++) {
          configuration.wifiPassword[i] = password.charAt(i);
          configuration.wifiPassword[i + 1] = 0;//password.charAt(i);
        }
        Serial.print("mode:");
        Serial.println(modee);
        Serial.print("SSID:");
        Serial.println(configuration.wifiSSID);
        Serial.print("password:");
        Serial.println(configuration.wifiPassword);
        configuration.wifiAPMode = modee == "ap";
        saveConfiguration(true);
        webServer.send_P(400, textHMTL, okReboot);
      } else {
        webServer.send_P(400, textHMTL, "mode must be one of client, ap.");
      }
    });

  }
}


void webServerSendOK() {
  webServer.send_P(200, textHMTL, "Ok !");
}

void webServerSendHome() {
  webServer.send_P(200, textHMTL, homeHTML);
}
