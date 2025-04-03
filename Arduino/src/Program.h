#pragma once

#include "Commands.h"

struct Command
{
    int number;
    CommandType type;
    union
    {
        MixCommand mix;
        GapCommand gap;
        AlarmCommand alarm;
    } data;
};

class CProgram
{
public:
    void start();
    void reset();
    const Command * next();
    const Command * setCommandNumber(int number);
    int operationCount() const;
    const Command *currentCommand() const;

private:
    const Command * readCurrentCommand();
    template <typename TCommandData>
    const Command * readCommandData();

    int m_commandCount = 0;
    int m_currentCommandNumber = 0;
    int m_currentCommandAddress = 0;
    int m_currentCommandSize = 0;
    Command m_currentCommand = {0, CommandType::Mix, MixCommand{}};
};