#include "ProgramLoader.hpp"
#include "ProgramExecutor.hpp"
#include "Timer.hpp"




enum class Mode 
{
  Load,
  Execute,
  Idle,
};

auto loadTimer = CSecondTimer();
auto programLoader = CProgramLoader();
auto programExecutor = CProgramExecutor();
auto mode = Mode::Load;
int  executorResult = SUCCESS;

void setup() {
  programLoader.setup();
  programLoader.setUpReadyToWrite();
  loadTimer.start(3);
}

void loop() {
  processChangeMode();
  processLoad();
  processExecute();
}

void processExecute()
{
  if (mode != Mode::Execute)
    return;
  executorResult = programExecutor.process();
}

void processLoad()
{
  if (mode != Mode::Load)
    return;
  programLoader.tryWrite();
}

void processChangeMode()
{
  const auto timerStaus = loadTimer.process();
  if (timerStaus == TimerStatus::Finished && mode == Mode::Load)
  {
    Serial.println("ExecuteStarted");
    mode = Mode::Execute;
    programExecutor.start();
  }
  if (executorResult == PROGRAM_FINISHED && mode == Mode::Execute)
  {
    Serial.println("ExecuteFinished");
    mode = Mode::Idle;
  }

}

