#pragma once

#include "IProcess.h"
#include "IButtonWatcher.h"
#include "Defines.hpp"
#include "Timer.h"

class CButtonWatcherProcess : public IProcess, public IButtonWatcher
{
public:
    ~CButtonWatcherProcess() override = default;
    // IProcess
    int start() override;
    int process() override;
    int stop() override;

    // IButtonWatcher
    void setup() override;
    bool clicked() override;
    bool longPressed() override;
    ButtonState state() override;

private:
    ButtonState readState();

    ButtonState m_lastState = ButtonState::Released;
    bool m_clicked = false;
    bool m_longPressed = false;
    CSecondTimer m_longPressTimer;
};