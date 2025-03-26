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
  CSecondTimer();

  void start(int seconds);
  void stop();
  TimerStatus process();

private:
  bool m_isAvtive = false;
  uint64_t m_startTime = 0;
  uint64_t m_duration_ms = 0;
};