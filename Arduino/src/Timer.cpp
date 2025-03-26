#include "Timer.h"

#include <Arduino.h>


void CTimer::startSeconds(int seconds)
{
  startMilliseconds(seconds * 1000);
}

void CTimer::startMilliseconds(int milliseconds)
{
  m_duration_ms = milliseconds;
  m_startTime = millis();
  m_currentStatus = TimerStatus::Running;
}

void CTimer::stop()
{
  m_currentStatus = TimerStatus::Idle;
  m_startTime = 0;
  m_duration_ms = 0;
}

void CTimer::pause()
{
  if (m_currentStatus != TimerStatus::Running)
    return;
  m_currentStatus = TimerStatus::Paused;
  const auto now = millis();
  m_duration_ms = m_startTime + m_duration_ms - now;
}

void CTimer::resume()
{
  if (m_currentStatus != TimerStatus::Paused)
    return;
  m_currentStatus = TimerStatus::Running;
  const auto now = millis();
  m_startTime = now;
}

TimerStatus CTimer::process()
{
  if (m_currentStatus == TimerStatus::Running)
  {
    const auto now = millis();
    if (m_startTime + m_duration_ms < now)
      m_currentStatus = TimerStatus::Finished;
  }
  return m_currentStatus;
}
