#pragma once

#include "IProcess.h"
#include "IIndicator.h"


class CProgramLoadProcess : public IProcess
{
  enum class LoadStatus;

public:
  CProgramLoadProcess(IIndicator &indicator);
  ~CProgramLoadProcess() override = default;

  void setup();

  // IProcess
  int process() override;
  int start() override;
  int stop() override;

private:
  void setLoadStatus(LoadStatus status);

  IIndicator& m_indicator;
  LoadStatus m_loadStatus;
};
