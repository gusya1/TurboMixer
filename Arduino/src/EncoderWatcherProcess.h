#pragma once

#include "IProcess.h"
#include "IEncoderWatcher.h"
#include "Defines.hpp"
#include "Timer.h"


class CEncoderWatcherProcess : public IProcess, public IEncoderWatcher
{
public:
    CEncoderWatcherProcess() = default;
    ~CEncoderWatcherProcess() override = default;

    // IProcess
    int start() override;
    int process() override;
    int stop() override;

    // 
    EncoderState state() const override;

    // own
    void setup();

private:
    bool m_lastStateCLK = false;
    EncoderState m_currentState = EncoderState::Idle;
};