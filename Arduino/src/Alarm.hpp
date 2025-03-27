#pragma once

#include "Defines.hpp"

#include <Arduino.h>


inline void alarm()
{
  tone(PIN_PIEZO, 1275, 1000);
}
