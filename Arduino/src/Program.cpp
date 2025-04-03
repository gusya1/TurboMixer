#include "Program.h"

#include "Commands.h"
#include "Defines.hpp"

#include <EEPROM.h>

namespace
{
    struct __attribute__((packed)) ProgramHeader
    {
        char prefix = STX;
        uint8_t commandCount = 0;
    };
}

void CProgram::start()
{
    reset();
    auto header = ProgramHeader();
    EEPROM.get(0, header);
    m_commandCount = header.commandCount;
    m_currentCommandNumber = 0;
    m_currentCommandAddress = sizeof(header);
    readCurrentCommand();
}

void CProgram::reset()
{
    m_commandCount = 0;
    m_currentCommandNumber = 0;
    m_currentCommandAddress = 0;
    m_currentCommandSize = 0;
}

const Command *CProgram::next()
{
    if (m_currentCommandNumber == m_commandCount - 1)
        return nullptr;
    m_currentCommandNumber++;
    m_currentCommandAddress += m_currentCommandSize;
    return readCurrentCommand();
}

const Command * CProgram::setCommandNumber(int number)
{
    start();
    while (number != m_currentCommandNumber)
        if (!next())
            return nullptr;
    return currentCommand();
}

int CProgram::operationCount() const
{
    return m_commandCount;
}

const Command *CProgram::currentCommand() const
{
    if (m_currentCommandSize != 0)
        return &m_currentCommand;
    return nullptr;
}

template <typename TCommandData>
const Command *CProgram::readCommandData()
{
    auto commandData = TCommandData{};
    EEPROM.get(m_currentCommandAddress, commandData);
    m_currentCommandSize = sizeof(commandData);
    m_currentCommand.number = m_currentCommandNumber;
    m_currentCommand.data = *reinterpret_cast<decltype(m_currentCommand.data) *>(&commandData);
    return &m_currentCommand;
}

const Command *CProgram::readCurrentCommand()
{
    m_currentCommand.type = CommandType(EEPROM.read(m_currentCommandAddress));

    switch (m_currentCommand.type)
    {
    case CommandType::Mix:
        return readCommandData<MixCommand>();
        break;
    case CommandType::Gap:
        return readCommandData<GapCommand>();
        break;
    case CommandType::Alarm:
        return readCommandData<AlarmCommand>();
        break;
    default:
        break;
    }
    return nullptr;
}
