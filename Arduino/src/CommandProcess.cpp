#include "CommandProcess.h"

#include "Defines.hpp"
#include "Timer.h"
#include "Alarm.hpp"
#include "ICommandProcess.h"

#include <Arduino.h>

CMixCommandProcess::CMixCommandProcess(IMixerController& mixerController, int power, int duration)
    : m_mixerController{mixerController}, m_power{power}, m_duration{duration}
{
}

int CMixCommandProcess::start()
{
  m_timer.startSeconds(m_duration);
  return m_mixerController.setMixPower(m_power);
}

void CMixCommandProcess::pause()
{
  m_timer.pause();
  m_mixerController.stop();
}

void CMixCommandProcess::resume()
{
  m_timer.resume();
  m_mixerController.setMixPower(m_power);
}

ProcessStatus CMixCommandProcess::process()
{
  const auto timerStaus = m_timer.process();
  if (timerStaus != TimerStatus::Finished)
    return ProcessStatus::Running;
  m_mixerController.stop();
  return ProcessStatus::Finished;
}

// **************************

CGapCommandProcess::CGapCommandProcess(int duration)
    : m_duration{duration}
{
}

int CGapCommandProcess::start()
{
  m_timer.startSeconds(m_duration);
  return SUCCESS;
}

void CGapCommandProcess::pause()
{
  m_timer.pause();
}

void CGapCommandProcess::resume()
{
  m_timer.resume();
}

ProcessStatus CGapCommandProcess::process()
{
  const auto timerStaus = m_timer.process();
  if (timerStaus != TimerStatus::Finished)
    return ProcessStatus::Running;
  return ProcessStatus::Finished;
}

// **************************

int CAlarmCommandProcess::start()
{
  return SUCCESS;
}

void CAlarmCommandProcess::pause()
{
}

void CAlarmCommandProcess::resume()
{
}

ProcessStatus CAlarmCommandProcess::process()
{
  alarm();
  return ProcessStatus::Finished;
}