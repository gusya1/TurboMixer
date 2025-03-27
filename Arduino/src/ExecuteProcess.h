#pragma once

#include "IProcess.h"


class IButtonWatcher;
class IIndicator;
class ICommandProcess;


class CExecuteProcess : public IProcess
{
  struct ExecuteResult;

public:
  CExecuteProcess(const IButtonWatcher&, IIndicator&);
  int start() override;
  int process() override;
  int stop() override;

private:
  int executeNextCommand();
  int startNewCommandProcess(ICommandProcess *newCommandProcess);
  void releaseCurrentCommandProcess();
  ExecuteResult executeCommand(int commandAddress);
  ExecuteResult executeMixCommand(int commandAddress);
  ExecuteResult executeGapCommand(int commandAddress);
  ExecuteResult executeAlarmCommand(int commandAddress);

  const IButtonWatcher& m_buttonWatcher;
  IIndicator& m_indicator;
  bool m_initialProcess = true;
  bool m_paused = false;

  int m_nextCommandNumber = 0;
  int m_nextCommandAddress = 0;
  int m_commandCount = 0;
  ICommandProcess *m_pCommandProcess = nullptr;
};
