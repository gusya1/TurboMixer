#include "ProgramLoadProcess.h"
#include "ExecuteProcess.h"
#include "ButtonWatcherProcess.h"
#include "IProcess.h"
#include "EncoderWatcherProcess.h"
#include "IdleProcess.hpp"
#include "IndicatorProcess.h"
#include "MixerController.hpp"

#include <Arduino.h>

enum class Mode
{
  Load,
  Execute,
  Idle,
};

auto g_indicatorProcess = CIndicatorProcess();
auto g_buttonWacherProcess = CButtonWatcherProcess(PIN_BUTTON);
auto g_switchButtonWacherProcess = CButtonWatcherProcess(PIN_ENC_SW);
auto g_encoderWatcherProcess = CEncoderWatcherProcess();
auto g_mixerController = CMixerController();

auto g_idleProcess = CIdleProcess(g_indicatorProcess);
auto g_programLoader = CProgramLoadProcess(g_indicatorProcess);
auto g_executeProcess = CExecuteProcess(g_buttonWacherProcess,
                                        g_switchButtonWacherProcess,
                                        g_encoderWatcherProcess,
                                        g_indicatorProcess,
                                        g_mixerController);

class CModeSwitcher
{
public:
  void start()
  {
    changeMode(g_idleProcess, Mode::Idle);
  }

  void process()
  {
    if (m_currentMode == Mode::Idle && g_buttonWacherProcess.longPressed())
      changeMode(g_programLoader, Mode::Load);
    if (m_currentMode == Mode::Idle && g_buttonWacherProcess.clicked())
      changeMode(g_executeProcess, Mode::Execute);

    auto result = m_pCurrentProcess->process();
    if (result == PROCESS_FINISHED)
      changeMode(g_idleProcess, Mode::Idle);
    else if (result != SUCCESS)
      returnCodeHandler(result);
  }

private:
  void changeMode(IProcess &newProcess, Mode mode)
  {
    const auto stopResult = m_pCurrentProcess->stop();
    if (stopResult != SUCCESS)
    {
      returnCodeHandler(stopResult);
      return;
    }

    m_currentMode = mode;
    m_pCurrentProcess = &newProcess;
    const auto stratResult = m_pCurrentProcess->start();
    if (stratResult != SUCCESS)
    {
      returnCodeHandler(stratResult);
      return;
    }
  }

  void returnCodeHandler(int returnCode)
  {
    changeMode(g_idleProcess, Mode::Idle);
  }

  IProcess *m_pCurrentProcess = &g_idleProcess;
  Mode m_currentMode = Mode::Idle;
};

auto g_modeSwitcher = CModeSwitcher();

void setup()
{
  g_buttonWacherProcess.setup();
  g_programLoader.setup();
  g_indicatorProcess.setup();
  g_encoderWatcherProcess.setup();
  g_switchButtonWacherProcess.setup();
  g_mixerController.setup();

  g_buttonWacherProcess.start();
  g_indicatorProcess.start();
  g_encoderWatcherProcess.start();
  g_switchButtonWacherProcess.start();

  g_modeSwitcher.start();
}

void loop()
{
  g_buttonWacherProcess.process();
  g_indicatorProcess.process();
  g_encoderWatcherProcess.process();
  g_switchButtonWacherProcess.process();

  g_modeSwitcher.process();
}
