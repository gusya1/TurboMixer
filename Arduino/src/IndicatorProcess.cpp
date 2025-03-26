#include "IndicatorProcess.h"

#include "IProcess.h"
#include "Timer.h"
#include "ShiftRegister.hpp"
#include "Defines.hpp"


namespace pattern_bits
{
    // 
    // |--A--|
    // |F   B|
    // |--G--|
    // |E   C|
    // |--D--| DP
    // 

    constexpr byte A = 1 << 2;
    constexpr byte B = 1 << 4;
    constexpr byte C = 1 << 5;
    constexpr byte D = 1 << 7;
    constexpr byte E = 1 << 6;
    constexpr byte F = 1 << 3;
    constexpr byte G = 1 << 1;
    constexpr byte DP = 1 << 0;
}

namespace
{
    byte getCharPattern(char pattern)
    {
        using namespace pattern_bits;
        switch (pattern)
        {
        case '0':
            return A | B | C | D | E | F;
        case '1':
            return B | C;
        case '2':
            return A | B | G | D | E;
        case '3':
            return A | B | G | D | C;
        case '4':
            return B | C | F | G;
        case '5':
            return A | C | D | F | G;
        case '6':
            return A | C | D | E | F | G;
        case '7':
            return A | B | C;
        case '8':
            return A | B | C | D | E | F | G;
        case '9':
            return A | B | C | D | F | G;
        case 'E':
            return A | D | E | F | G;
        case 'P':
            return A | B | E | F | G;
        default:
            break;
        }
        return 0;
    }
    
    byte getDigitPattern(int digit)
    {
        digit = digit % 10;
        return getCharPattern(digit + 48);
    }
}

int CIndicatorProcess::start() 
{
    m_leftPattern = 0;
    m_rightPattern = 0;
    switchDigit();
    return SUCCESS;
}

int CIndicatorProcess::process()
{
    const auto timerStatus = m_digitSwitchTimer.process();
    if (timerStatus == TimerStatus::Finished)
        switchDigit();
    
    m_shiftRegister.writeData(m_currentDigitIsLeft?m_leftPattern:m_rightPattern);
    return SUCCESS;
}

int CIndicatorProcess::stop()
{
    m_leftPattern = 0;
    m_rightPattern = 0;
    m_digitSwitchTimer.stop();
    return SUCCESS;
}

void CIndicatorProcess::setup()
{
    m_shiftRegister.setup();
    pinMode(PIN_IND_DIGIT_SWITCH, OUTPUT);
}

void CIndicatorProcess::setNumber(int number)
{
    const auto ones = number % 10;
    m_rightPattern = getDigitPattern(ones);
    const auto tens = (number / 10) % 10;
    m_leftPattern = (tens != 0)? getDigitPattern(tens): 0;
}

void CIndicatorProcess::switchDigit()
{
    m_currentDigitIsLeft = !m_currentDigitIsLeft;
    digitalWrite(PIN_IND_DIGIT_SWITCH, m_currentDigitIsLeft?LOW:HIGH);
    m_digitSwitchTimer.startMilliseconds(INDICATOR_DIGIT_SWITCH_DURATION_MSEC);
}