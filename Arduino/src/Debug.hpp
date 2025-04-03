#include "Defines.hpp"

#include <Arduino.h>

#ifdef DEBUG
#define DEBUG_MSG(msg) Serial.println(msg)
#else
#define DEBUG_MSG(msg)
#endif