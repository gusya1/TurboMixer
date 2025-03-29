#pragma once

#include "Timer.h"
#include "ICommandProcess.h"
#include "IMixerController.h"

#include <Arduino.h>


class CMixCommandProcess : public ICommandProcess
{
public:
  CMixCommandProcess(IMixerController& mixerController, int power, int duration);
  ~CMixCommandProcess() override = default;

  // ICommandProcess
  int start() override;
  void pause() override;
  void resume() override;
  ProcessStatus process() override;

private:
  IMixerController& m_mixerController;
  CTimer m_timer;
  int m_power;
  int m_duration;
};

class CGapCommandProcess : public ICommandProcess
{
public:
  explicit CGapCommandProcess(int duration);
  ~CGapCommandProcess() override = default;

  // ICommandProcess
  int start() override;
  void pause() override;
  void resume() override;
  ProcessStatus process() override;

private:
  CTimer m_timer;
  int m_duration;
};

class CAlarmCommandProcess : public ICommandProcess
{
public:
  CAlarmCommandProcess() = default;
  ~CAlarmCommandProcess() override = default;

  int start() override;
  void pause() override;
  void resume() override;
  ProcessStatus process() override;
};