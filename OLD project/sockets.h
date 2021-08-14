#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include "OTA.h"
#include "functions.h"


void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
  case WStype_ERROR:
    USE_SERIAL.printf("[WSc] Error!! : %s\n", payload);
    break;
  case WStype_DISCONNECTED:
    USE_SERIAL.printf("[WSc] Disconnected!\n");
    break;
  case WStype_CONNECTED: 
    {
      DynamicJsonDocument doc(1024);
      USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
      // send message to server when Connected
      doc["message"] = "connect";
      doc["deviceId"] = macAddress;
      doc["deviceApp"] = DEVICE_APP;
      doc["deviceType"] = DEVICE_TYPE;
      doc["updateURL"] = updateURL;
      doc["version"] = version;
      String JsonToSend = "";
      serializeJson(doc, JsonToSend);
      webSocket.sendTXT(JsonToSend);
    }
    break;
  case WStype_TEXT:
    {
      gotMessageSocket(payload);
    }
    break;
  case WStype_BIN:
    USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
    hexdump(payload, length);
    // send data to server
    // webSocket.sendBIN(payload, length);
    break;
  case WStype_FRAGMENT_TEXT_START:
    break;
  case WStype_FRAGMENT_BIN_START:
    break;
  case WStype_FRAGMENT:
    break;
  case WStype_FRAGMENT_FIN:
    break;
  case WStype_PING:
    break;
  case WStype_PONG:
    break;
  }
}