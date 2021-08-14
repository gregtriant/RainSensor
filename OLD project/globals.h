#pragma once
#include <Adafruit_BME280.h>
#include "I2CScanner.h"

#define USE_SERIAL Serial
#define DEVICE_APP "Test"                        // change
#define DEVICE_TYPE "Wemos D1 mini"           
// #define DEVICE_TYPE "Wemos D1"           

float version = 0.2;                             // change
char hostUrl[] = "192.168.0.87"; // socket host  // change 192.168.0.87
int port = 80; // socket port                    // change
String mac;
char macAddress[20];
char updateURL[100] = "http://192.168.0.87/update/files/firmware.bin";  // change

I2CScanner scanner;
ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

// --------- //
// user code //
// --------- //
Adafruit_BME280 bme;
//code end