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

    // own
    void setup();

private:
    void switchDigit();

    CShiftRegister<PIN_IND_SER, PIN_IND_RCLK, PIN_IND_SRCLK> m_shiftRegister;
    CTimer m_digitSwitchTimer;
    bool m_currentDigitIsLeft = true;
    byte m_leftPattern = 0;
    byte m_rightPattern = 0;
};