#pragma once

#include <stdint.h>


enum class TimerStatus
{
  Idle = 0,
  Running,
  Paused,
  Finished,
};

class CTimer
{
public:
  CTimer() = default;

  void startSeconds(int seconds);
  void startMilliseconds(int milliseconds);
  void stop();
  void pause();
  void resume();
  TimerStatus process();

private:
  TimerStatus m_currentStatus = TimerStatus::Idle;
  uint64_t m_startTime = 0;
  uint64_t m_duration_ms = 0;
};