#include <WiFi.h>
#include <HTTPClient.h>
#include "CloudManager.h" 
#include "Config.h"
#include "Globals.h"
#include "Sensor.h" 

void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) { delay(100); } 
  Serial.println(" Connected!");
}

void getFanSpeedFromFirebase() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    // URL UPDATED HERE
    String url = "https://" + String(DATABASE_URL) + "/Fan%20Number/" + String(FAN_ID) + "/Fan%20Speed.json?auth=" + String(DATABASE_SECRET);
    http.begin(url);
    if (http.GET() > 0) {
      String payload = http.getString();
      if(payload != "null" && payload != "") { currentFanSpeed = payload.toInt(); }
    }
    http.end();
  }
}

void checkLockStatusFromFirebase() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    // URL UPDATED HERE
    String url = "https://" + String(DATABASE_URL) + "/Fan%20Number/" + String(FAN_ID) + "/Status.json?auth=" + String(DATABASE_SECRET);
    http.begin(url);
    if (http.GET() > 0) {
      String payload = http.getString();
      payload.replace("\"", ""); 
      payload.trim();

      if (payload == "Normal" && isLocked) {
        isLocked = false;
        digitalWrite(LED_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("\n*** SYSTEM UNLOCKED VIA FIREBASE ***");
        Serial.println("-> Recalibrating baseline...");
        baselineDistance = getDistance(); 
        
        // URL UPDATED HERE
        String url2 = "https://" + String(DATABASE_URL) + "/Fan%20Number/" + String(FAN_ID) + "/Suicide.json?auth=" + String(DATABASE_SECRET);
        http.begin(url2);
        http.addHeader("Content-Type", "application/json");
        http.PUT("false");
        http.end();
        
        Serial.println("-> System Normal. Resuming fan operations.");
      } 
      else if (payload == "Locked" && !isLocked) {
        isLocked = true;
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);
        Serial.println("\n!!! BOOTED INTO LOCKED STATE !!!");
      }
    }
    http.end();
  }
}

void triggerPanicMode() {
  isLocked = true;
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  Serial.println("\n!!! EMERGENCY DETECTED !!!");
  Serial.println("-> MOTOR KILLED.");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // URL UPDATED HERE
    String url1 = "https://" + String(DATABASE_URL) + "/Fan%20Number/" + String(FAN_ID) + "/Suicide.json?auth=" + String(DATABASE_SECRET);
    http.begin(url1);
    http.addHeader("Content-Type", "application/json");
    http.PUT("true");
    http.end();

    // URL UPDATED HERE
    String url2 = "https://" + String(DATABASE_URL) + "/Fan%20Number/" + String(FAN_ID) + "/Status.json?auth=" + String(DATABASE_SECRET);
    http.begin(url2);
    http.addHeader("Content-Type", "application/json");
    http.PUT("\"Locked\"");
    http.end();
    
    Serial.println("-> CLOUD UPDATED: SYSTEM LOCKED.");
  }
}