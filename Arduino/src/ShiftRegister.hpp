#pragma once

#include <Arduino.h>

/// 74hc595
///   1 23
///  IIIIIIII
///  +=======
///  IIIIIIII
///

template<int SER, int RCLK, int SRCLK>
class CShiftRegister
{
public:
    void setup()
    {
        pinMode(SER, OUTPUT);
        pinMode(RCLK, OUTPUT);
        pinMode(SRCLK, OUTPUT);
    }

    void writeData(byte data)
    {
        digitalWrite(RCLK, LOW);
        shiftOut(SER, SRCLK, LSBFIRST, data);
        digitalWrite(RCLK, HIGH);
    }
};