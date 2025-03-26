#include "Timer.h"

#include <Arduino.h>

CSecondTimer::CSecondTimer() {}

void CSecondTimer::start(int seconds)
{
  m_duration_ms = seconds * 1000;
  m_startTime = millis();
  m_currentStatus = TimerStatus::Running;
}

void CSecondTimer::stop()
{
  m_currentStatus = TimerStatus::Idle;
  m_startTime = 0;
  m_duration_ms = 0;
}

void CSecondTimer::pause()
{
  if (m_currentStatus != TimerStatus::Running)
    return;
  m_currentStatus = TimerStatus::Paused;
  const auto now = millis();
  m_duration_ms = m_startTime + m_duration_ms - now;
}

void CSecondTimer::resume()
{
  if (m_currentStatus != TimerStatus::Paused)
    return;
  m_currentStatus = TimerStatus::Running;
  const auto now = millis();
  m_startTime = now;
}

TimerStatus CSecondTimer::process()
{
  if (m_currentStatus == TimerStatus::Running)
  {
    const auto now = millis();
    if (m_startTime + m_duration_ms < now)
      m_currentStatus = TimerStatus::Finished;
  }
  return m_currentStatus;
}
