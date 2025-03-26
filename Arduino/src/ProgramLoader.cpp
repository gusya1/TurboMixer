#include "ProgramLoader.h"

#include "Defines.hpp"
#include "IProcess.h"

#include <EEPROM.h>
#include <Arduino.h>

namespace
{
  class CProgramWriter
  {
  public:
    bool write(char *data, int len)
    {
      if (EEPROM.length() < m_address + len)
        return false;
      for (int i = 0; i < len; i++)
        EEPROM.update(m_address + i, data[i]);
      m_address += len;
      return true;
    }

  private:
    uint16_t m_address = 0;
  };

  bool loadProgram()
  {
    auto writer = CProgramWriter();

    auto endReceived = false;
    auto readTimeout = false;
    auto EEPROMWriteStatus = true;
    char buffer[BUFFER_LEN];
    while (!endReceived && !readTimeout && EEPROMWriteStatus)
    {
      auto len = Serial.readBytes(buffer, BUFFER_LEN);
      endReceived = buffer[len - 1] == ETX;
      if (len == 0)
        readTimeout = true;
      EEPROMWriteStatus = writer.write(buffer, len);
    }
    return endReceived && EEPROMWriteStatus;
  }
}

enum class CProgramLoader::LoadStatus
{
  Idle = 0,
  Ready,
  Loading,
  Success,
  Fail
};

CProgramLoader::CProgramLoader() : m_loadStatus{LoadStatus::Idle}
{
}

void CProgramLoader::setup()
{
  Serial.begin(9600);
  Serial.setTimeout(100);
}

int CProgramLoader::process()
{
  if (Serial.peek() != STX)
    return SUCCESS;
  setLoadStatus(LoadStatus::Loading);
  // TODO make partial loading
  auto ok = loadProgram();
  if (ok)
    setLoadStatus(LoadStatus::Success);
  else
    setLoadStatus(LoadStatus::Fail);
  return PROCESS_FINISHED;
}

int CProgramLoader::start()
{
  setLoadStatus(LoadStatus::Ready);
  return SUCCESS;
}

int CProgramLoader::stop()
{
  if (m_loadStatus == LoadStatus::Loading)
    return LOADING_NOT_FINISHED;
  setLoadStatus(LoadStatus::Idle);
  return SUCCESS;
}

void CProgramLoader::setLoadStatus(LoadStatus status)
{
  m_loadStatus = status;
  switch (m_loadStatus)
  {
  case LoadStatus::Ready:
    Serial.println(LOAD_STATUS_READY);
    break;
  case LoadStatus::Loading:
    Serial.println(LOAD_STATUS_LOADING);
    break;
  case LoadStatus::Success:
    Serial.println(LOAD_STATUS_SUCCESS);
    break;
  case LoadStatus::Fail:
    Serial.println(LOAD_STATUS_FAIL);
    break;
  default:
    break;
  }
}
