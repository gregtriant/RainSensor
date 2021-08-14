#pragma once
#include <Arduino.h>
#include "pins.h"

#define MAX_RESET_TIME 10*60*1000 // 10 mins
// Counting rain ticks
static volatile unsigned int rainTicks = 0;
unsigned long timeBetweenTicks = 0;
unsigned long lastRainInterrupt = 0;
unsigned long timeUntilReset = MAX_RESET_TIME; // 10 mins
float rainBPM = 0.0f;
bool gotRainInterrupt = false;
bool startedGettingRainTicks = false;

IRAM_ATTR void interruptRain(){
  gotRainInterrupt = true;
}

void resetRainTicks() {
  rainTicks = 0;
  timeBetweenTicks = 0;
  lastRainInterrupt = 0;
  gotRainInterrupt = false;
  timeUntilReset = MAX_RESET_TIME;
}

void rainTicksSetup() {
  pinMode(rainPin, INPUT);
  resetRainTicks();
  attachInterrupt(digitalPinToInterrupt(rainPin), interruptRain, CHANGE);
}

void gotRainTickLoop() {
  if(gotRainInterrupt == true) {
    if (startedGettingRainTicks) {
      timeBetweenTicks = millis() - lastRainInterrupt;
      
      // calculate timeUntilReset
      if (timeBetweenTicks < 1*60*1000) { // 1 min
        timeUntilReset = 2*60*1000; // 2 mins
      } else if (timeBetweenTicks < 5*60*1000) { // 5 mins
        timeUntilReset = timeBetweenTicks * 1.4;
      } else {
        timeUntilReset = MAX_RESET_TIME;
      }

      if (timeBetweenTicks < 200) {
        Serial.println("Debouncing interrupt");
      } else if (timeBetweenTicks > timeUntilReset) {
        Serial.println("Reseting RainTicks");
        resetRainTicks();
        // but we got a new Tick so we start counting again
        Serial.println("Started");
        startedGettingRainTicks = true; // so that it calculates the times next time
      } else {
        rainTicks++;
        Serial.print("Ticks: ");
        Serial.println(rainTicks);
        Serial.print("Time between ticks: ");
        Serial.println(timeBetweenTicks);
        rainBPM = 60000/timeBetweenTicks;
        Serial.print("Rain BPM: ");
        Serial.println(rainBPM);
        Serial.print("Time until reset: ");
        Serial.println(timeUntilReset);
      }
    } else {
      Serial.println("Started");
      startedGettingRainTicks = true; // so that it calculates the times next time
    }
    gotRainInterrupt = false;
    lastRainInterrupt = millis();
  }
}


