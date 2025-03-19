#pragma once

#include "Defines.hpp"
#include "Timer.hpp"
#include "Mixer.hpp"
#include "Alarm.hpp"

#include <Arduino.h>

enum class ProcessStatus
{
  Running = 0,
  Finished,
};

class ICommandProcess
{
public:
  virtual ~ICommandProcess() = default;
  virtual int start() = 0;
  virtual ProcessStatus process() = 0;
};


class CMixCommandProcess: public ICommandProcess
{
public:
  CMixCommandProcess(int power, int duration)
    : m_power{power}, m_duration{duration}
  {}

  ~CMixCommandProcess() override = default;
  
  int start() override {
    m_timer.start(m_duration);
    return setMixPower(m_power);
  }

  ProcessStatus process() override
  {
    const auto timerStaus = m_timer.process();
    if (timerStaus != TimerStatus::Finished)
      return ProcessStatus::Running;
    stopMixer();
    return ProcessStatus::Finished;
  }

private:
  CSecondTimer m_timer;
  int m_power;
  int m_duration;
};


class CGapCommandProcess: public ICommandProcess
{
public:
  CGapCommandProcess(int duration)
    : m_duration{duration}
  {}

  ~CGapCommandProcess() override = default;
  
  int start() override {
    m_timer.start(m_duration);
    return SUCCESS;
  }

  ProcessStatus process() override
  {
    const auto timerStaus = m_timer.process();
    if (timerStaus != TimerStatus::Finished)
      return ProcessStatus::Running;
    return ProcessStatus::Finished;
  }

private:
  CSecondTimer m_timer;
  int m_duration;
};

class CAlarmCommandProcess: public ICommandProcess
{
public:
  CAlarmCommandProcess() {}

  ~CAlarmCommandProcess() override = default;
  
  int start() override {
    return SUCCESS;
  }

  ProcessStatus process() override
  {
    alarm();
    return ProcessStatus::Finished;
  }

private:
  CSecondTimer m_timer;
  int m_duration;
};