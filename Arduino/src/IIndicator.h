#pragma once

class IIndicator
{
public:
    virtual ~IIndicator()= default;

    virtual void setNumber(int) = 0;
    virtual void setChars(const char chars[2]) = 0;
    virtual void resetIndicator() = 0;
};