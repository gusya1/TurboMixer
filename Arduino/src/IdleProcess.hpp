#pragma once

#include "IProcess.h"
#include "IIndicator.h"
#include "Defines.hpp"

class CIdleProcess : public IProcess
{

public:
    CIdleProcess(IIndicator &indicator)
        : m_indicator{indicator}
    {
    }
    ~CIdleProcess() override = default;

    int process() override { return SUCCESS; }
    int start() override
    {
        m_indicator.setChars("rE");
        return SUCCESS;
    }
    int stop() override
    {
        m_indicator.resetIndicator();
        return SUCCESS;
    }

private:
    IIndicator &m_indicator;
};