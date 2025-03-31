#include "EncoderWatcherProcess.h"

#include "IProcess.h"
#include "Defines.hpp"
#include "Timer.h"

#include <Arduino.h>

int CEncoderWatcherProcess::start()
{
    m_lastStateCLK = false;
    m_currentState = EncoderState::Idle;
    return SUCCESS;
}

int CEncoderWatcherProcess::process()
{
    auto currentStateCLK = digitalRead(PIN_ENC_CLK);
    if (currentStateCLK != m_lastStateCLK && currentStateCLK == 1)
        if (digitalRead(PIN_ENC_DT) != currentStateCLK)
            m_currentState = EncoderState::TurnedCCW;
        else
            m_currentState = EncoderState::TurnedCW;
    else
        m_currentState = EncoderState::Idle;
    m_lastStateCLK = currentStateCLK; 
    return SUCCESS;
}

int CEncoderWatcherProcess::stop()
{
    return SUCCESS;
}

void CEncoderWatcherProcess::setup()
{
    pinMode(PIN_ENC_CLK, INPUT);
    pinMode(PIN_ENC_DT, INPUT);
}

EncoderState CEncoderWatcherProcess::state() const
{
    return m_currentState;
}