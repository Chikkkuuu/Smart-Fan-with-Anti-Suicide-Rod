#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

// 'extern' tells the compiler these variables are created in sketch.ino, but can be used anywhere
extern float baselineDistance;
extern volatile int currentFanSpeed; 
extern bool isLocked;

#endif