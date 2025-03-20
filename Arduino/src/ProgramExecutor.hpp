#pragma once

#include "Defines.hpp"
#include "CommandProcess.hpp"

#include <EEPROM.h>
#include <Arduino.h>

enum class CommandType : uint8_t
{
  Mix = 0,
  Gap = 1,
  Alarm = 2
};

struct __attribute__((packed)) ProgramHeader
{
  char prefix = STX;
  uint8_t commandCount = 0;
};

struct __attribute__((packed)) MixCommand
{
  CommandType commandType = CommandType::Mix;
  uint8_t argsCount = 0;
  int32_t power = 0;
  int32_t duration = 0;
};

struct __attribute__((packed)) GapCommand
{
  CommandType commandType = CommandType::Gap;
  uint8_t argsCount = 0;
  int32_t duration = 0;
};

struct __attribute__((packed)) AlarmCommand
{
  CommandType commandType = CommandType::Alarm;
  uint8_t argsCount = 0;
};

class CProgramExecutor
{
public:
  int start()
  {
    auto header = ProgramHeader();
    EEPROM.get(0, header);
    m_commandCount = header.commandCount;
    m_nextCommandNumber = 0;
    m_nextCommandAddress = sizeof(header);
    return executeNextCommand();
  }

  int process()
  {
    if (!m_pCommandProcess)
      return SUCCESS;
    const auto status = m_pCommandProcess->process();
    if (status == ProcessStatus::Finished)
      return executeNextCommand();
    return SUCCESS;
  }

private:
  int executeNextCommand()
  {
    if (m_nextCommandNumber == m_commandCount)
      return PROGRAM_FINISHED;

    m_nextCommandNumber++;
    return executeCommand(m_nextCommandAddress);
  }

  int executeCommand(int commandAddress)
  {
    const auto commandType = CommandType(EEPROM.read(commandAddress));
    switch (commandType)
    {
    case CommandType::Mix:
      return executeMixCommand(commandAddress);
    case CommandType::Gap:
      return executeGapCommand(commandAddress);
    case CommandType::Alarm:
      return executeAlarmCommand(commandAddress);
    default:
      return PROGRAM_READ_ERROR;
    }
  }

  int executeMixCommand(int commandAddress)
  {
    auto command = MixCommand();
    EEPROM.get(commandAddress, command);

    if (command.commandType != CommandType::Mix)
      return PROGRAM_READ_ERROR;

    m_nextCommandAddress += sizeof(command);
    return startNewCommandProcess(new CMixCommandProcess(command.power, command.duration));
  }

  int executeGapCommand(int commandAddress)
  {
    auto command = GapCommand();
    EEPROM.get(commandAddress, command);

    if (command.commandType != CommandType::Gap)
      return PROGRAM_READ_ERROR;

    m_nextCommandAddress += sizeof(command);
    return startNewCommandProcess(new CGapCommandProcess(command.duration));
  }

  int executeAlarmCommand(int commandAddress)
  {
    auto command = AlarmCommand();
    EEPROM.get(commandAddress, command);

    if (command.commandType != CommandType::Alarm)
      return PROGRAM_READ_ERROR;

    m_nextCommandAddress += sizeof(command);
    return startNewCommandProcess(new CAlarmCommandProcess());
  }

  int startNewCommandProcess(ICommandProcess *newCommandProcess)
  {
    if (m_pCommandProcess)
      delete m_pCommandProcess;
    m_pCommandProcess = newCommandProcess;
    return m_pCommandProcess->start();
  }

  int m_nextCommandNumber = 0;
  int m_nextCommandAddress = 0;
  int m_commandCount = 0;
  ICommandProcess *m_pCommandProcess = nullptr;
};
