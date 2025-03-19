#pragma once

#include <Arduino.h>


enum class TimerStatus
{
  Running = 0,
  Finished,
};


class CSecondTimer
{
public:
  CSecondTimer()
  {

  }

  void start(int seconds)
  {
    m_duration_ms = seconds * 1000;
    m_startTime = millis();
  }

  TimerStatus process()
  {
    auto now = millis();
    if (m_startTime + m_duration_ms < now)
      return TimerStatus::Finished;
    return TimerStatus::Running;
  }

private:
  uint64_t m_startTime = 0;
  uint64_t m_duration_ms = 0;
};