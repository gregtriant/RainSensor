#include <WiFiManager.h>
#include "SocketClient.h"
#include <Adafruit_BME280.h>
#include <Wire.h>
#include "pins.h"
#include "rainTicks.h"

SocketClient testClient;
Adafruit_BME280 bme;


String defineDataToSend() {
  String stringToSend = "";
  // float temp = bme.readTemperature();      
  // float hum = bme.readHumidity();          
  // float pres = bme.readPressure() * 0.01f; 
  // stringToSend += "temp: " + String(temp) + " hum: " + String(hum) + " pres: " + String(pres) + " counter: " + counter;
  stringToSend += "rainTicks:  " + String(rainTicks);
  return stringToSend;
}

void recievedData(String data) {
  Serial.println(data);
  // counter++;
  if (data.equals("reset")) {
    rainTicks = 0;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFiManager wifiManager;
  // wifiManager.resetSettings();
  wifiManager.autoConnect("ESPAutoConnectAP");

  // test client
  testClient.setHostPort(80);
  testClient.setSocketHostURL("sensordata.space");  //192.168.0.87
  testClient.setVersion(0.3);
  testClient.setDeviceApp("Socket testing");
  testClient.setDeviceType("Wemos D1 mini");
  testClient.setUpdateURL("http://192.168.0.87/update/files/firmware.bin");
  testClient.setDataToSendFunciton(defineDataToSend);
  testClient.setRecievedDataFunciton(recievedData);
  testClient.init();

  bme.begin(0x77);
  rainTicksSetup();
}

void loop() {
  testClient.loop();
  gotRainTickLoop();
}