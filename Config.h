#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- CREDENTIALS ---
#define WIFI_SSID "Wokwi-GUEST" 
#define WIFI_PASSWORD ""
#define DATABASE_URL "smart-fan-with-anti-suicide-default-rtdb.asia-southeast1.firebasedatabase.app"
#define DATABASE_SECRET "QV48fa4GHFmilyuTywfEDCVyjPkO6TyWRUEEwJle" 

// --- DEVICE IDENTITY ---
#define FAN_ID "001" // <--- ADD THIS LINE! Change this when flashing a new board.

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