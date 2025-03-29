#pragma once

#include "IProcess.h"
#include "Timer.h"
#include "ShiftRegister.hpp"
#include "Defines.hpp"
#include "IIndicator.h"


class CIndicatorProcess : public IProcess, public IIndicator
{
public:
    CIndicatorProcess() = default;
    ~CIndicatorProcess() override = default;

    // IProcess
    int start() override;
    int process() override;
    int stop() override;

    // IIndicator
    void setNumber(int) override;
    void setChars(const char chars[2]) override;
    void resetIndicator() override;

    // own
    void setup();

private:
    void changeState();

    CShiftRegister<PIN_COMMON_SER, PIN_IND_RCLK, PIN_COMMON_SRCLK> m_shiftRegister;
    CTimer m_digitSwitchTimer;
    int m_currentState = 0;
    byte m_leftPattern = 0;
    byte m_rightPattern = 0;
};