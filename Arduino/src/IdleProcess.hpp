#pragma once

#include "IProcess.h"
#include "Defines.hpp"

class CIdleProcess : public IProcess
{

public:
    ~CIdleProcess() override = default;

    int process() override { return SUCCESS; }
    int start() override { return SUCCESS; }
    int stop() override { return SUCCESS; }
};