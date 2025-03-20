#pragma once

#include "Defines.hpp"

#include <Arduino.h>

int setMixPower(int power)
{
  if (power > 6 || power < 1)
    return INCORRECT_POWER_ERROR;
  Serial.print("Mix ");
  Serial.println(power);
  return SUCCESS;
}

int stopMixer()
{
  Serial.println("Mix stopped");
  return SUCCESS;
}
