#pragma once

#include "IProcess.h"
#include "Defines.hpp"
#include "Timer.h"

enum class ButtonState
{
    Released = 0,
    Pressed,
};

class CButtonWatcherProcess : public IProcess
{
public:
    // IProcess
    int start() override;
    int process() override;
    int stop() override;

    // own
    void setup();
    bool clicked();
    bool longPressed();
    ButtonState state();

private:
    ButtonState readState();

    ButtonState m_lastState = ButtonState::Released;
    bool m_clicked = false;
    bool m_longPressed = false;
    CSecondTimer m_longPressTimer;
};