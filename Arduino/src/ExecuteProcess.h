#pragma once

#include "IProcess.h"
#include "Program.h"
#include "Commands.h"

class IButtonWatcher;
class IEncoderWatcher;
class IIndicator;
class ICommandProcess;
class IMixerController;

class CExecuteProcess : public IProcess
{
  enum class ExecuteState
  {
    Running = 0,
    Paused,
    ChooseOperation,
    Finished
  };

public:
  CExecuteProcess(const IButtonWatcher &pauseButtonWatcher,
                  const IButtonWatcher &changeStateButtonWatcher,
                  const IEncoderWatcher &,
                  IIndicator &,
                  IMixerController &);
  int start() override;
  int process() override;
  int stop() override;

private:
  void checkStateProcess();
  void pauseProcess();
  void chooseOperationProcess();
  void indicatorProcess();
  void operationProcess();
  void setCurrentCommandNumberOnIndicator();

  void executeNextCommand();
  void startNewCommandProcess(ICommandProcess *newCommandProcess);
  void releaseCurrentCommandProcess();
  void executeCommand(const Command &);
  void executeMixCommand(const MixCommand &);
  void executeGapCommand(const GapCommand &);
  void executeAlarmCommand(const AlarmCommand &);

  const IButtonWatcher &m_pauseButtonWatcher;
  const IButtonWatcher &m_changeStateButtonWatcher;
  const IEncoderWatcher &m_encoder;
  IIndicator &m_indicator;
  IMixerController &m_mixerController;
  bool m_initialProcess = true;
  ExecuteState m_state;
  int m_chosenOperation = 0;

  CProgram m_program;
  ICommandProcess *m_pCommandProcess = nullptr;
};
