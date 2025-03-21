#pragma once

#include "IProcess.h"


class CProgramLoader : public IProcess
{
  enum class LoadStatus;

public:
  CProgramLoader();
  ~CProgramLoader() override = default;

  void setup();

  // IProcess
  int process() override;
  int start() override;
  int stop() override;

private:
  void setLoadStatus(LoadStatus status);

  LoadStatus m_loadStatus;
};
