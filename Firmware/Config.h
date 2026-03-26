#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- CREDENTIALS ---
#define WIFI_SSID "<wifi_name>" 
#define WIFI_PASSWORD "<wifi_password>"
#define DATABASE_URL "<database_url>"
#define DATABASE_SECRET "<database_secret>" 

// --- DEVICE IDENTITY ---
#define FAN_ID "001" 

// --- PIN DEFINITIONS ---
#define TRIG_PIN 4
#define ECHO_PIN 5
#define LED_PIN 6
#define DIR_PIN 7
#define BUZZER_PIN 9
#define STEP_PIN 10 

// --- TIMING CONSTANTS ---
const unsigned long CALIBRATION_INTERVAL = 86400000; 
const unsigned long HTTP_INTERVAL = 500; 
const unsigned long LOCK_CHECK_INTERVAL = 2000; 

#endif
