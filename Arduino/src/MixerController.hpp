#pragma once

#include "Defines.hpp"
#include "ShiftRegister.hpp"
#include "IMixerController.h"

#include <Arduino.h>

class CMixerController : public IMixerController
{
public:
  ~CMixerController() = default;

  // IMixerController
  int setMixPower(int power) override;
  int stop() override;

  // own
  void setup();

private:
  CShiftRegister<PIN_COMMON_SER, PIN_MIXER_RCLK, PIN_COMMON_SRCLK> m_shiftRegister;
};

int CMixerController::setMixPower(int power)
{
  if (power > 6 || power < 1)
    return INCORRECT_POWER_ERROR;
  byte pattern = byte(0b10000000) >> (power - 1);
  m_shiftRegister.writeData(pattern);
  return SUCCESS;
}

int CMixerController::stop()
{
  m_shiftRegister.writeData(0);
  return SUCCESS;
}

void CMixerController::setup()
{
  m_shiftRegister.setup();
  m_shiftRegister.writeData(0);
}