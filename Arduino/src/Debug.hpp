#include <Arduino.h>

//#define DEBUG

#ifdef DEBUG
    #define DEBUG_MSG(msg) Serial.println(msg)
#else
    #define DEBUG_MSG(msg) 
#endif