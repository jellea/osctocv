#include <ESP8266WebServer.h>

ESP8266WebServer webServer(80);

const char* form PROGMEM  = "<html>\n<title>oscpixi</title>\n<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' crossorigin='anonymous'>\n<div class='container' style='width:300px;margin20px;padding:20px'>\n<h1>oscpixi</h1>\n<h2>Rest API</h2>\n<h3>inputs</h3>\n<form action='/in' target='resultinput'>\n  <div class='form-group'>\n    <label for='indexinput'>index</label>\n   <select name='index' id='indexinput'  class='form-control'>\n     <option value=1>1</option>\n      <option value=2>2</option>\n      <option value=3>3</option>\n      <option value=4>4</option>\n      <option value=5>5</option>\n      <option value=6>6</option>\n      <option value=7>7</option>\n      <option value=8>8</option>\n      <option value=9>9</option>\n      <option value=10>10</option>\n      <option value=11>11</option>\n      <option value=12>12</option>\n      <option value=13>13</option>\n      <option value=14>14</option>\n      <option value=15>15</option>\n      <option value=16>16</option>\n      <option value=17>17</option>\n      <option value=18>18</option>\n      <option value=19>19</option>\n      <option value=20>20</option>\n    </select>\n </div>\n  <div class='form-group'>\n    <label for='inputMode'>mode</label>\n   <select name='mode' id='indexMode'  class='form-control'>\n     <option value='trig'>trigger</option>\n     <option value='gate'>gate</option>\n      <option value='cvuni'>unipolar cv (0v +10v)</option>\n      <option value='cvbi'>biploar cv (-5v +5v)</option>\n    </select>\n </div>\n  <input type='submit'/>\n</form>\n<iframe src='' name='resultinput' style='border:none;width:250px;height:40px;'></iframe>\n\n<h3>outputs</h3>\n<form action='/out' target='resultoutput'>\n <div class='form-group'>\n    <label for='indexoutput'>index</label>\n    <select name='index' id='indexoutput'  class='form-control'>\n      <option value=1>1</option>\n      <option value=2>2</option>\n      <option value=3>3</option>\n      <option value=4>4</option>\n      <option value=5>5</option>\n      <option value=6>6</option>\n      <option value=7>7</option>\n      <option value=8>8</option>\n      <option value=9>9</option>\n      <option value=10>10</option>\n      <option value=11>11</option>\n      <option value=12>12</option>\n      <option value=13>13</option>\n      <option value=14>14</option>\n      <option value=15>15</option>\n      <option value=16>16</option>\n      <option value=17>17</option>\n      <option value=18>18</option>\n      <option value=19>19</option>\n      <option value=20>20</option>\n    </select>\n </div>\n  <div class='form-group'>\n    <label for='modeoutput'>mode</label>\n    <select name='mode' id='modeoutput'  class='form-control'>\n      <option value='trig'>trigger</option>\n     <option value='gate'>gate</option>\n      <option value='cvuni'>unipolar cv (0v +10v)</option>\n      <option value='cvbi'>biploar cv (-5v +5v)</option>\n    </select>\n </div>\n  <div class='form-group'>\n    <label for='value'>value</label>\n    <input name='value' id='value'  class='form-control'/>\n  </div>\n  <input type='submit'/>\n  </form>\n <iframe src='' name='resultoutput' style='border:none;width:250px;height:40px;'></iframe>\n \n  <h2>OSC Messages</h2>\n <p>port 5000<p>\n <p>/out/&lt;1-20&gt;/&lt;trig|gate|cvuni|cvbi&gt; float</p>\n <p>/in/&lt;1-20&gt;/&lt;trig|gate|cvuni|cvbi&gt;</p>\n  \n  <h2>RTP MIDI</h2>\n <p>todo</p>\n \n  <h3>WIFI Config</h3>\n<form action='/wifi' target='wifioutput'>\n <div class='form-group'>\n    <label for='wifiMode'>mode</label>\n    <select name='mode' id='wifiMode'  class='form-control'>\n      <option value='client'>client</option>\n      <option value='ap'>ap</option>\n    </select>\n </div>\n  <div class='form-group'>\n    <label for='ssid'>ssid (client)</label>\n   <input name='mode' id='ssid'  class='form-control'/>\n  </div>\n  <div class='form-group'>\n    <label for='password'>password (client)</label>\n   <input name='password' id='password'  class='form-control'/>\n  </div>\n  <input type='submit'/>\n  </form>\n <iframe src='' name='wifioutput' style='border:none;width:250px;height:40px;'></iframe>\n <p>\n   in AP mode you will find your device as 'oscpixi-xxx'. \n   the wifi password in AP mode is 0101010101\n  <p>\n</div>\n<html>";
const char* textHMTL PROGMEM = "text/html";


void handleWebClient() {
  if (configuration.restEnabled) {
    webServer.handleClient();
  }
}

void startWebServer() {
  if (configuration.restEnabled) {
    Serial.println("Rest API on port 80");
    webServer.begin();
  }
}

void createWebServerRoutes() {

  if (configuration.restEnabled) {
    webServer.on("/out", []() {
      String modee =  webServer.arg("mode");
      int i = webServer.arg("index").toInt();
      float value = webServer.arg("value").toFloat();

      if (i > 0 && i <= 20) {
        if (modee == "trig") {
          if (value > 1 || value < 0) {
            webServer.send(200, textHMTL, "value must be between 0 and 1");
          } else {
            setChannel(i, OUTPUT_MODE_TRIG, value);
            sendOK();
          }
        } else  if (modee == "gate") {
          if (value > 1 || value < 0) {
            webServer.send(200, textHMTL, "value must be between 0 and 1");
          } else {
            setChannel(i, OUTPUT_MODE_GATE, value);
            sendOK();
          }
        } else  if (modee == "cvuni") {
          if (value > 1 || value < 0) {
            webServer.send(200, textHMTL, "value must be between 0 and 1");
          } else {
            setChannel(i, OUTPUT_MODE_CVUNI, value);
            sendOK();
          }
        } else  if (modee == "cvbi") {
          if (value > 1 || value < -1) {
            webServer.send(200, textHMTL, "value must be between -1 and 1");
          } else {
            setChannel(i, OUTPUT_MODE_CVBI, value);
            sendOK();
          }
        } else {
          webServer.send(200, textHMTL, "mode must be one of trig, gate, cvuni, cvbi");
        }

      } else {
        webServer.send(200, textHMTL, "index must be between 1 and 20");
      }

    });

    webServer.on("/in", []() {
      String modee =  webServer.arg("mode");
      int i = webServer.arg("index").toInt();

      if (i > 0 && i <= 20) {
        if (modee == "trig") {
          setChannel(i, INPUT_MODE_TRIG, 0);
          sendOK();
        } else  if (modee == "gate") {
          setChannel(i, INPUT_MODE_GATE, 0);
          sendOK();
        } else  if (modee == "cvuni") {
          setChannel(i, INPUT_MODE_CVUNI, 0);
          sendOK();
        } else  if (modee == "cvbi") {
          setChannel(i, INPUT_MODE_CVBI, 0);
          sendOK();
        } else {
          webServer.send(200, textHMTL, "mode must be one of trig, gate, cvuni, cvbi");
        }
      } else {
        webServer.send(200, textHMTL, "index must be between 1 and 20");
      }

    });

    webServer.on("/wifi", []() {
      String modee =  webServer.arg("mode");
      String ssid =  webServer.arg("ssid");
      String password =  webServer.arg("login");
      if (modee == "ap" || modee  == "client") {
        for (int i = 0; i < ssid.length(); i++) {
          configuration.wifiSSID[i] = ssid.charAt(i);
          configuration.wifiSSID[i + 1] = ssid.charAt(i);
        }
        for (int i = 0; i < password.length(); i++) {
          configuration.wifiPassword[i] = password.charAt(i);
          configuration.wifiPassword[i + 1] = password.charAt(i);
        }
        Serial.print("mode:");
        Serial.println(modee);
        Serial.print("SSID:");
        Serial.println(configuration.wifiSSID);
        Serial.print("password:");
        Serial.println(configuration.wifiPassword);
        configuration.wifiAPMode = modee == "ap";
        saveConfiguration(true);
        webServer.send(200, textHMTL, "ok. power cycle your device.");
      } else {
        webServer.send(200, textHMTL, "mode must be one of client, ap.");
      }
    });

    webServer.on("/", []() {
      webServer.send(200, textHMTL, form);
    });
    webServer.on("/blank", []() {
      webServer.send(200, textHMTL, "");
    });
  }
}


void sendOK() {
  webServer.send(200, textHMTL, "Ok !");
}

void sendHome() {
  //TODO:
  webServer.send_P(200, textHMTL, form);
}
