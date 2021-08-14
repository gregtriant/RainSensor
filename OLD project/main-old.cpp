#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266httpUpdate.h>
#include <WebSocketsClient.h>
#include "Ticker.h"
#include <Hash.h>
// needed for WiFiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
// end
#include "globals.h"
#include "OTA.h"
#include "sockets.h"

//---------------------//
// your code goes here //
// ------------------- //


void toggleLed();
Ticker timerToggleLed(toggleLed, 1000);
// code stop

void setup() {
  Serial.begin(115200);
  USE_SERIAL.begin(115200);
  delay(500);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // turn LED off

  // scanner.Init(); // scan for i2c devices
  // scanner.Scan();

  mac = WiFi.macAddress();
  mac.toCharArray(macAddress, 50);

  bme.begin(0x77);

  webSocket.begin(hostUrl, port, "/"); // server address, port and URL
  webSocket.onEvent(webSocketEvent);   // initialte our event handler
  // webSocket.setAuthorization("user", "Password"); // use HTTP Basic Authorization this is optional remove if not needed
  webSocket.setReconnectInterval(5000); // try ever 5000 again if connection has failed
  
  //---------------------//
  // your code goes here //
  // ------------------- //
  timerToggleLed.start();

  // wifi manager config
  WiFiManager wifiManager;
  // wifiManager.resetSettings();
  wifiManager.autoConnect("ESPAutoConnectAP");
  // code stop
}

void loop() {
  webSocket.loop();
  
  //---------------------//
  // your code goes here //
  // ------------------- //
  timerToggleLed.update();
  // code stop
  delay(10);
}

//---------------------//
// your code goes here //
// ------------------- //
void toggleLed() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}