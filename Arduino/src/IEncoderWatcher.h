#pragma once

enum class EncoderState
{
    Idle = 0,
    TurnedCW,
    TurnedCCW
};

class IEncoderWatcher
{
public:
    virtual ~IEncoderWatcher() = default;

    virtual EncoderState state() const = 0;
};