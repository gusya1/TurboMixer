#pragma once

class IProcess
{
public:
    virtual ~IProcess() = default;

    virtual int process() = 0;
    virtual int start() = 0;
    virtual int stop() = 0;
};