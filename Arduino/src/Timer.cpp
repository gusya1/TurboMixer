#include "Timer.h"

#include <Arduino.h>

CSecondTimer::CSecondTimer() {}

void CSecondTimer::start(int seconds)
{
  m_duration_ms = seconds * 1000;
  m_startTime = millis();
  m_isAvtive = true;
}

void CSecondTimer::stop()
{
  m_isAvtive = false;
}

TimerStatus CSecondTimer::process()
{
  if (!m_isAvtive)
    return TimerStatus::Idle;

  auto now = millis();
  if (m_startTime + m_duration_ms < now)
    return TimerStatus::Finished;
  return TimerStatus::Running;
}