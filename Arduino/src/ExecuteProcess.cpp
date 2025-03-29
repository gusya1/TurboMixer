#include "ExecuteProcess.h"

#include "Defines.hpp"
#include "CommandProcess.h"
#include "ICommandProcess.h"
#include "IButtonWatcher.h"
#include "IIndicator.h"
#include "Debug.hpp"

#include <EEPROM.h>
#include <Arduino.h>

namespace
{
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
}

struct CExecuteProcess::ExecuteResult
{
  int returnCode;
  uint32_t commandSize;
};

CExecuteProcess::CExecuteProcess(const IButtonWatcher &buttonWatcher, IIndicator &indicator, IMixerController &mixerController)
    : m_buttonWatcher{buttonWatcher}, m_indicator{indicator}, m_mixerController{mixerController}
{
}

int CExecuteProcess::start()
{
  auto header = ProgramHeader();
  EEPROM.get(0, header);
  m_commandCount = header.commandCount;
  m_nextCommandNumber = 0;
  m_nextCommandAddress = sizeof(header);

  m_initialProcess = true;

  return executeNextCommand();
}

int CExecuteProcess::process()
{
  DEBUG_MSG("CExecuteProcess::process");
  if (!m_pCommandProcess)
    return SUCCESS;

  if (m_buttonWatcher.clicked() && !m_initialProcess)
  {
    m_paused = !m_paused;
    if (m_paused)
    {
      m_pCommandProcess->pause();
      m_indicator.setChars("PA");
    }
    else
    {
      m_pCommandProcess->resume();
      m_indicator.setNumber(m_nextCommandNumber - 1);
    }
  }

  m_initialProcess = false;

  const auto status = m_pCommandProcess->process();
  if (status == ProcessStatus::Finished)
    return executeNextCommand();
  return SUCCESS;
}

int CExecuteProcess::stop()
{
  releaseCurrentCommandProcess();
  m_nextCommandNumber = 0;
  m_nextCommandAddress = sizeof(ProgramHeader);
  return SUCCESS;
}

int CExecuteProcess::executeNextCommand()
{
  if (m_nextCommandNumber == m_commandCount)
  {
    m_indicator.resetIndicator();
    return PROGRAM_FINISHED;
  }

  m_indicator.setNumber(m_nextCommandNumber);

  const auto result = executeCommand(m_nextCommandAddress);

  m_nextCommandNumber++;
  m_nextCommandAddress += result.commandSize;
  return result.returnCode;
}

CExecuteProcess::ExecuteResult CExecuteProcess::executeCommand(int commandAddress)
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
    return {PROGRAM_READ_ERROR, 0};
  }
}

CExecuteProcess::ExecuteResult CExecuteProcess::executeMixCommand(int commandAddress)
{
  auto command = MixCommand();
  EEPROM.get(commandAddress, command);

  if (command.commandType != CommandType::Mix)
    return {PROGRAM_READ_ERROR, 0};

  return {startNewCommandProcess(new CMixCommandProcess(m_mixerController, command.power, command.duration)), sizeof(command)};
}

CExecuteProcess::ExecuteResult CExecuteProcess::executeGapCommand(int commandAddress)
{
  auto command = GapCommand();
  EEPROM.get(commandAddress, command);

  if (command.commandType != CommandType::Gap)
    return {PROGRAM_READ_ERROR, 0};

  return {startNewCommandProcess(new CGapCommandProcess(command.duration)), sizeof(command)};
}

CExecuteProcess::ExecuteResult CExecuteProcess::executeAlarmCommand(int commandAddress)
{
  auto command = AlarmCommand();
  EEPROM.get(commandAddress, command);

  if (command.commandType != CommandType::Alarm)
    return {PROGRAM_READ_ERROR, 0};

  return {startNewCommandProcess(new CAlarmCommandProcess()), sizeof(command)};
}

int CExecuteProcess::startNewCommandProcess(ICommandProcess *newCommandProcess)
{
  releaseCurrentCommandProcess();
  m_pCommandProcess = newCommandProcess;
  return m_pCommandProcess->start();
}

void CExecuteProcess::releaseCurrentCommandProcess()
{
  if (m_pCommandProcess)
    delete m_pCommandProcess;
  m_pCommandProcess = nullptr;
}
