#pragma once

#include "CommandProcess.hpp"
#include "IProcess.h"

class CExecuteProcess : public IProcess
{
  struct ExecuteResult;

public:
  int start() override;
  int process() override;
  int stop() override;

private:
  int executeNextCommand();
  int startNewCommandProcess(ICommandProcess *newCommandProcess);
  ExecuteResult executeCommand(int commandAddress);
  ExecuteResult executeMixCommand(int commandAddress);
  ExecuteResult executeGapCommand(int commandAddress);
  ExecuteResult executeAlarmCommand(int commandAddress);

  int m_nextCommandNumber = 0;
  int m_nextCommandAddress = 0;
  int m_commandCount = 0;
  ICommandProcess *m_pCommandProcess = nullptr;
};
