#include "ButtonWatcherProcess.h"

#include "IProcess.h"
#include "Defines.hpp"
#include "Timer.hpp"

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
        m_longPressTimer.start(LONG_PRESS_BUTTON_DURATION);

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


bool CButtonWatcherProcess::clicked()
{
    return m_clicked;
}

bool CButtonWatcherProcess::longPressed()
{
    return m_longPressed;
}

ButtonState CButtonWatcherProcess::state()
{
  return m_lastState;
}

void CButtonWatcherProcess::setup()
{
    pinMode(PIN_BUTTON, INPUT);
}

ButtonState CButtonWatcherProcess::readState()
{
    return ButtonState(digitalRead(PIN_BUTTON));
}
