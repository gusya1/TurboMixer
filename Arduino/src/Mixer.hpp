#pragma once

#include "Defines.hpp"

#include <Arduino.h>

inline int setMixPower(int power)
{
  if (power > 6 || power < 1)
    return INCORRECT_POWER_ERROR;
  Serial.print("Mix ");
  Serial.println(power);
  return SUCCESS;
}

inline int stopMixer()
{
  Serial.println("Mix stopped");
  return SUCCESS;
}
