#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <sstream>
#include <Wire.h>
#include "globals.h"
#include "sockets.h"

// function declarations
void sendDataWithSocket(DynamicJsonDocument doc);
void getDataFromSocket(DynamicJsonDocument recievedDoc); // TODO

// function for the user
void defineDataToSend(JsonObject dataJson) {
  // example
  //{
  //   hum: '',
  //   temp: '',
  //   pres: ''
  //}
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();
  float pres = bme.readPressure() * 0.01f;
  dataJson["temp"] = temp;
  dataJson["hum"] = hum;
  dataJson["pres"] = pres;
}

// Functions
void gotMessageSocket(uint8_t * payload) {
  DynamicJsonDocument doc(300);
  USE_SERIAL.printf("[WSc] got data: %s\n", payload);
  deserializeJson(doc, payload);
  const char* serverMessage = doc["message"];
  // update
  if (strcmp(serverMessage, "update") == 0) {
    const char* url = doc["url"];
    sprintf(updateURL, "%s", url);
    // Serial.println(updateURL);
    updatingMode();
  }

  if (strcmp(serverMessage, "sendData") == 0) {
    sendDataWithSocket(doc);
  }
}

void sendDataWithSocket(DynamicJsonDocument recievedDoc) {
  DynamicJsonDocument responseDoc(1024);
  const char *recieverId = recievedDoc["recieverId"];

  responseDoc["message"] = "returningData";
  responseDoc["recieverId"] = recieverId;

  JsonObject dataJson = responseDoc.createNestedObject("dataJson");
  
  defineDataToSend(dataJson); // call user function to fill the JsonObject
  
  String JsonToSend = "";
  serializeJson(responseDoc, JsonToSend);
  Serial.println("");
  Serial.println(JsonToSend);
  webSocket.sendTXT(JsonToSend);
}

void getDataFromSocket(DynamicJsonDocument recievedDoc) {
  // TODO
}

