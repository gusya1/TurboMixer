#pragma once


class IMixerController
{
public:
  virtual ~IMixerController() = default;

  virtual int setMixPower(int power) = 0;
  virtual int stop() = 0;
};