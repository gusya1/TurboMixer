#include "ProgramLoader.h"
#include "ExecuteProcess.h"
#include "ButtonWatcherProcess.h"
#include "IProcess.h"
#include "IdleProcess.hpp"
#include "Timer.hpp"

enum class Mode
{
  Load,
  Execute,
  Idle,
};

auto g_idleProcess = CIdleProcess();
auto g_programLoader = CProgramLoader();
auto g_executeProcess = CExecuteProcess();
auto g_buttonWacherProcess = CButtonWatcherProcess();

// при долгом нажатии на кнопку устройство переключается на режим приёма программы
bool g_buttonLongPressed = false;
// при клике на кнопку запускается 
bool g_buttonClicked = false;

class CModeSwitcher
{
public:
  void process()
  {
    if (m_currentMode != Mode::Load && g_buttonWacherProcess.longPressed())
      changeMode(g_programLoader, Mode::Load);
    if (m_currentMode != Mode::Execute && g_buttonWacherProcess.clicked())
      changeMode(g_executeProcess, Mode::Execute);

    auto result = m_currentProcess.process();
    if (result == PROCESS_FINISHED)
      changeMode(g_idleProcess, Mode::Idle);
    else if (result != SUCCESS)
      returnCodeHandler(result);
  }

private:
  void changeMode(IProcess& newProcess, Mode mode)
  {
    const auto stopResult = m_currentProcess.stop();
    if (stopResult != SUCCESS)
    {
      returnCodeHandler(stopResult);
      return;
    }

    m_currentMode = mode;
    m_currentProcess = newProcess;
    const auto stratResult = m_currentProcess.start();
    if (stratResult != SUCCESS)
    {
      returnCodeHandler(stratResult);
      return;
    }
  }

  void returnCodeHandler(int returnCode)
  {

  }

  IProcess& m_currentProcess = g_idleProcess;
  Mode m_currentMode = Mode::Idle;
};

auto g_modeSwitcher = CModeSwitcher();



void setup()
{
  g_buttonWacherProcess.setup();
  g_programLoader.setup();
}

void loop()
{
  g_buttonWacherProcess.process();
  g_modeSwitcher.process();
}
