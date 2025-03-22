#pragma once

#include <Arduino.h>

enum class TimerStatus
{
  Idle = 0,
  Running,
  Finished,
};

class CSecondTimer
{
public:
  CSecondTimer() {}

  void start(int seconds)
  {
    m_duration_ms = seconds * 1000;
    m_startTime = millis();
    m_isAvtive = true;
  }

  void stop()
  {
    m_isAvtive = false;
  }

  TimerStatus process()
  {
    if (!m_isAvtive)
      return TimerStatus::Idle;

    auto now = millis();
    if (m_startTime + m_duration_ms < now)
      return TimerStatus::Finished;
    return TimerStatus::Running;
  }

private:
  bool m_isAvtive = false;
  uint64_t m_startTime = 0;
  uint64_t m_duration_ms = 0;
};