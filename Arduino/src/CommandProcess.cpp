#include "CommandProcess.h"

#include "Defines.hpp"
#include "Timer.h"
#include "Mixer.hpp"
#include "Alarm.hpp"
#include "ICommandProcess.h"

#include <Arduino.h>

CMixCommandProcess::CMixCommandProcess(int power, int duration)
    : m_power{power}, m_duration{duration}
{
}

int CMixCommandProcess::start()
{
  m_timer.start(m_duration);
  return setMixPower(m_power);
}

ProcessStatus CMixCommandProcess::process()
{
  const auto timerStaus = m_timer.process();
  if (timerStaus != TimerStatus::Finished)
    return ProcessStatus::Running;
  stopMixer();
  return ProcessStatus::Finished;
}

// **************************

CGapCommandProcess::CGapCommandProcess(int duration)
    : m_duration{duration}
{
}

int CGapCommandProcess::start()
{
  m_timer.start(m_duration);
  return SUCCESS;
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

ProcessStatus CAlarmCommandProcess::process()
{
  alarm();
  return ProcessStatus::Finished;
}