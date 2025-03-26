#include "ButtonWatcherProcess.h"

#include "IProcess.h"
#include "Defines.hpp"
#include "Timer.h"

#include <Arduino.h>

int CButtonWatcherProcess::start()
{
    m_lastState = readState();
    m_clicked = false;
    return SUCCESS;
}

int CButtonWatcherProcess::process()
{
    m_clicked = false;
    auto state = readState();

    if (state == ButtonState::Released && m_lastState == ButtonState::Pressed)
    {
        m_clicked = true;
        m_longPressed = false;
        m_longPressTimer.stop();
    }

    if (state == ButtonState::Pressed && m_lastState == ButtonState::Released)
        m_longPressTimer.startSeconds(LONG_PRESS_BUTTON_DURATION_SEC);

    if (m_longPressTimer.process() == TimerStatus::Finished)
        m_longPressed = true;

    m_lastState = state;
    return SUCCESS;
}

int CButtonWatcherProcess::stop()
{
    m_lastState = readState();
    m_clicked = false;
    m_longPressed = false;
    return SUCCESS;
}

bool CButtonWatcherProcess::clicked() const
{
    return m_clicked;
}

bool CButtonWatcherProcess::longPressed() const
{
    return m_longPressed;
}

ButtonState CButtonWatcherProcess::state() const
{
    return m_lastState;
}

void CButtonWatcherProcess::setup()
{
    pinMode(PIN_BUTTON, INPUT_PULLUP);
}

ButtonState CButtonWatcherProcess::readState()
{
    return digitalRead(PIN_BUTTON) ? ButtonState::Released : ButtonState::Pressed;
}
