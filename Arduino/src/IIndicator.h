#pragma once

class IIndicator
{
public:
    virtual ~IIndicator()= default;

    virtual void setNumber(int) = 0;
};