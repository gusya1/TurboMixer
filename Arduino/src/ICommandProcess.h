#pragma once

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
  virtual void pause() = 0;
  virtual void resume() = 0;
  virtual ProcessStatus process() = 0;
};