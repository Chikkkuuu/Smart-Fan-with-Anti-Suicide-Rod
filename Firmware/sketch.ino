#include <Arduino.h>
#include "Config.h"
#include "Globals.h"
#include "Sensor.h"
#include "CloudManager.h"

float baselineDistance = 0;
volatile int currentFanSpeed = 0; 
bool isLocked = false; 

int previousFanSpeed = -1; 
unsigned long lastCalibrationTime = 0;
unsigned long lastHttpTime = 0;
unsigned long lastLockCheckTime = 0;
hw_timer_t * timer = NULL;

void IRAM_ATTR onTimer() {
  if (currentFanSpeed > 0 && !isLocked) {
    digitalWrite(STEP_PIN, HIGH);
    for (int i = 0; i < 50; i++) { asm volatile("nop"); }
    digitalWrite(STEP_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  
  setupHardware();
  setupWiFi();

  Serial.println("Checking Firebase for security status...");
  checkLockStatusFromFirebase();

  if (!isLocked) {
    baselineDistance = getDistance();
    Serial.print("Baseline Distance: ");
    Serial.println(baselineDistance);
  }

  timer = timerBegin(1000000); 
  timerAttachInterrupt(timer, &onTimer); 
  timerAlarm(timer, 2000, true, 0); 
}

void loop() {
  if (isLocked) {
    if (millis() - lastLockCheckTime >= LOCK_CHECK_INTERVAL) {
      lastLockCheckTime = millis();
      checkLockStatusFromFirebase();
    }
    return; 
  }

  float currentDist = getDistance();
  if (currentDist > (baselineDistance + 15.0)) { 
    triggerPanicMode(); 
    return; 
  }

  if (millis() - lastHttpTime >= HTTP_INTERVAL) {
    lastHttpTime = millis();
    getFanSpeedFromFirebase(); 
  }

  if (currentFanSpeed != previousFanSpeed) {
    previousFanSpeed = currentFanSpeed;
    if (currentFanSpeed > 0) {
      int newDelay = map(currentFanSpeed, 1, 5, 2000, 200); 
      timerAlarm(timer, newDelay, true, 0);
    } else {
      timerAlarm(timer, 1000000, true, 0); 
    }
  }

  if (millis() - lastCalibrationTime >= CALIBRATION_INTERVAL) {
    baselineDistance = getDistance(); 
    lastCalibrationTime = millis();
  }
}
