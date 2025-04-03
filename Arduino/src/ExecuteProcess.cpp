#include "ExecuteProcess.h"

#include "Defines.hpp"
#include "CommandProcess.h"
#include "ICommandProcess.h"
#include "IButtonWatcher.h"
#include "IIndicator.h"
#include "Debug.hpp"
#include "IEncoderWatcher.h"

CExecuteProcess::CExecuteProcess(const IButtonWatcher &pauseButtonWatcher,
                                 const IButtonWatcher &changeStateButtonWatcher,
                                 const IEncoderWatcher &encoder,
                                 IIndicator &indicator,
                                 IMixerController &mixerController)
    : m_pauseButtonWatcher{pauseButtonWatcher},
      m_changeStateButtonWatcher{changeStateButtonWatcher},
      m_encoder{encoder},
      m_indicator{indicator},
      m_mixerController{mixerController},
      m_state{ExecuteState::Paused}
{
}

int CExecuteProcess::start()
{
  m_initialProcess = true;
  m_state = ExecuteState::Running;
  m_program.start();
  if (const auto pCommand = m_program.currentCommand())
    executeCommand(*pCommand);
  return SUCCESS;
}

int CExecuteProcess::process()
{
  checkStateProcess();
  chooseOperationProcess();
  pauseProcess();
  operationProcess();
  indicatorProcess();
  
  if (m_state == ExecuteState::Finished)
    return PROCESS_FINISHED;
  return SUCCESS;
}

void CExecuteProcess::checkStateProcess()
{
  if (m_pauseButtonWatcher.clicked() && !m_initialProcess)
  {
    switch (m_state)
    {
    case ExecuteState::Paused:
      m_state = ExecuteState::Running;
      m_pCommandProcess->resume();
      break;
    case ExecuteState::Running:
      m_state = ExecuteState::Paused;
      m_pCommandProcess->pause();
      break;
    default:
      break;
    }
  }
  if (m_changeStateButtonWatcher.state() == ButtonState::Pressed && m_state == ExecuteState::Paused)
    m_state = ExecuteState::ChooseOperation;
  m_initialProcess = false;
}

void CExecuteProcess::chooseOperationProcess()
{
  if (m_state != ExecuteState::ChooseOperation)
    return;
  switch (m_encoder.state())
  {
  case EncoderState::TurnedCW:
    if (m_chosenOperation < m_program.operationCount())
      m_chosenOperation++;
    break;
  case EncoderState::TurnedCCW:
    if (m_chosenOperation > 0)
      m_chosenOperation--;
    break;
  default:
    break;
  }
  if (m_changeStateButtonWatcher.state() != ButtonState::Pressed)
  {
    if (const auto pCommand = m_program.setCommandNumber(m_chosenOperation))
      executeCommand(*pCommand);
    m_state = ExecuteState::Paused;
  }
}

void CExecuteProcess::indicatorProcess()
{
  if (m_state == ExecuteState::Paused)
    m_indicator.setChars("PA");
  if (m_state == ExecuteState::Running)
    setCurrentCommandNumberOnIndicator();
  if (m_state == ExecuteState::ChooseOperation)
    m_indicator.setNumber(m_chosenOperation);
}

void CExecuteProcess::operationProcess()
{
  if (!m_pCommandProcess)
    return;
  const auto status = m_pCommandProcess->process();
  if (status == ProcessStatus::Finished)
    executeNextCommand();
}

void CExecuteProcess::pauseProcess()
{
  if (m_state != ExecuteState::Paused)
    return;
  if (!m_pCommandProcess)
    return;
  m_pCommandProcess->pause();
}

void CExecuteProcess::setCurrentCommandNumberOnIndicator()
{
  m_indicator.setNumber(m_chosenOperation);
}

int CExecuteProcess::stop()
{
  releaseCurrentCommandProcess();
  m_program.reset();
  return SUCCESS;
}

void CExecuteProcess::executeNextCommand()
{
  if (const auto pCommand = m_program.next())
    executeCommand(*pCommand);
  else
    m_state = ExecuteState::Finished;
}

void CExecuteProcess::executeCommand(const Command &command)
{
  DEBUG_MSG(command.number);
  m_chosenOperation = command.number;
  switch (command.type)
  {
  case CommandType::Mix:
    executeMixCommand(command.data.mix);
    break;
  case CommandType::Gap:
    executeGapCommand(command.data.gap);
    break;
  case CommandType::Alarm:
    executeAlarmCommand(command.data.alarm);
    break;
  default:
    break;
  }
}

void CExecuteProcess::executeMixCommand(const MixCommand &command)
{
  startNewCommandProcess(new CMixCommandProcess(m_mixerController, command.power, command.duration));
}

void CExecuteProcess::executeGapCommand(const GapCommand &command)
{
  startNewCommandProcess(new CGapCommandProcess(command.duration));
}

void CExecuteProcess::executeAlarmCommand(const AlarmCommand &)
{
  startNewCommandProcess(new CAlarmCommandProcess());
}

void CExecuteProcess::startNewCommandProcess(ICommandProcess *newCommandProcess)
{
  releaseCurrentCommandProcess();
  m_pCommandProcess = newCommandProcess;
  m_pCommandProcess->start();
}

void CExecuteProcess::releaseCurrentCommandProcess()
{
  if (m_pCommandProcess)
    delete m_pCommandProcess;
  m_pCommandProcess = nullptr;
}
