#pragma once

#include "IProcess.h"
#include "IButtonWatcher.h"
#include "Defines.hpp"
#include "Timer.h"

class CButtonWatcherProcess : public IProcess, public IButtonWatcher
{
public:
    CButtonWatcherProcess() = default;
    ~CButtonWatcherProcess() override = default;

    // IProcess
    int start() override;
    int process() override;
    int stop() override;

    // IButtonWatcher
    bool clicked() const override;
    bool longPressed() const override;
    ButtonState state() const override;

    // own
    void setup();

private:
    ButtonState readState();

    ButtonState m_lastState = ButtonState::Released;
    bool m_clicked = false;
    bool m_longPressed = false;
    CTimer m_longPressTimer;
};