/* Original Developer: Motea Marius © 2019*/
/* Updated 2021 by 641i130 */

// Libraries needed:
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#endif  // ESP8266
#if defined(ESP32)
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Update.h>
#endif  // ESP32
#include <WiFiUdp.h>
//#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "index.h" //html page

//// User configuration space for AC library classes ######################################

#include <ir_Midea.h>  //  replace library based on your AC unit model, check https://github.com/crankyoldgit/IRremoteESP8266/src

#define AUTO_MODE kMideaACAuto
#define COOL_MODE kMideaACCool
#define DRY_MODE kMideaACDry
#define HEAT_MODE kMideaACHeat
#define FAN_MODE kMideaACFan

#define FAN_AUTO kMideaACFanAuto
#define FAN_MIN kMideaACFanLow
#define FAN_MED kMideaACFanMed
#define FAN_HI kMideaACFanHigh

char deviceName[] = "AC Remote Control"; // Initial SSID name for setup
// ESP8266 GPIO pin to use for IR blaster.
const uint16_t kIrLed = 4; // Leave default if following the wiring guide in the Wiki
// Library initialization, change it according to the imported library file.
IRMideaAC ac(kIrLed);
ESP8266WiFiMulti wifiMulti; // Comment this if using WiFiManager
//// End user configuration ###############################################################

struct state {
  uint8_t temperature = 70, fan = 0, operation = 0; // Set default temp here for webpage to show
  bool powerStatus;
};
// core
state acState;
// settings

#if defined(ESP8266)
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdateServer;
#endif  // ESP8266
#if defined(ESP32)
WebServer server(80);
#endif  // ESP32

void handleRoot() {
  String s = page; //Read HTML contents
  server.send(200, "text/html", s);
}
void stateput() {
    DynamicJsonDocument root(1024);
    DeserializationError error = deserializeJson(root, server.arg("plain"));
    if (error) {
      server.send(404, "text/plain", "FAIL. " + server.arg("plain"));
    } else {
      if (root.containsKey("temp")) {
        acState.temperature = (uint8_t) root["temp"];
      }
      if (root.containsKey("fan")) {
        acState.fan = (uint8_t) root["fan"];
      }
      if (root.containsKey("power")) {
        acState.powerStatus = root["power"];
      }
      if (root.containsKey("mode")) {
        acState.operation = root["mode"];
      }
      String output;
      serializeJson(root, output);
      server.send(200, "text/plain", output);
      delay(200);
      if (acState.powerStatus) {
        ac.on();
        ac.setTemp(acState.temperature);
        if (acState.operation == 0) {
          ac.setMode(AUTO_MODE);
          ac.setFan(FAN_AUTO);
          acState.fan = 0;
        } else if (acState.operation == 1) {
          ac.setMode(COOL_MODE);
        } else if (acState.operation == 2) {
          ac.setMode(DRY_MODE);
        } else if (acState.operation == 3) {
          ac.setMode(HEAT_MODE);
        } else if (acState.operation == 4) {
          ac.setMode(FAN_MODE);
        }
        if (acState.operation != 0) {
          if (acState.fan == 0) {
            ac.setFan(FAN_AUTO);
          } else if (acState.fan == 1) {
            ac.setFan(FAN_MIN);
          } else if (acState.fan == 2) {
            ac.setFan(FAN_MED);
          } else if (acState.fan == 3) {
            ac.setFan(FAN_HI);
          }
        }
      } else {
        ac.off();
      }
      ac.send();
    }
}
void stateget() {
  DynamicJsonDocument root(1024);
  root["mode"] = acState.operation;
  root["fan"] = acState.fan;
  root["temp"] = acState.temperature;
  root["power"] = acState.powerStatus;
  String output;
  serializeJson(root, output);
  server.send(200, "text/plain", output);
}
void reset() {
  server.send(200, "text/html", "reset");
  delay(100);
  ESP.restart();
}
void setup() {
  Serial.begin(115200);
  Serial.println();
  ac.begin();
  delay(1000);
  // Uncomment this if using WiFiManager.h 
  // WiFiManager wifiManager; 
  #if defined(ESP8266)
    httpUpdateServer.setup(&server);
  #endif
  wifiMulti.addAP("my_other_wifi", "pass1");
  wifiMulti.addAP("Wifi", "pass");
  Serial.println("Connecting");
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Un-comment if using WiFiManager.h
  /*
  if (!wifiManager.autoConnect(deviceName)) {
    delay(3000);
    ESP.restart();
    delay(5000);
  }
  */
  server.on("/state", HTTP_PUT, stateput);
  server.on("/", handleRoot);
  server.on("/state", HTTP_GET, stateget);
  server.on("/reset", reset);
  server.begin();
}
void loop() {
  server.handleClient();
}
