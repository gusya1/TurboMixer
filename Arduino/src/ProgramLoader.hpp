#pragma once

#include "Defines.hpp"

#include <EEPROM.h>
#include <Arduino.h>

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

class CProgramLoader
{
public:
  CProgramLoader() {}

  void setup()
  {
    Serial.begin(9600);
    Serial.setTimeout(100);
  }

  void setUpReadyToWrite()
  {
    Serial.println(LOAD_STATUS_READY);
  }

  void tryWrite()
  {
    if (Serial.peek() != STX)
      return;
    Serial.println(LOAD_STATUS_LOADING);

    auto loadStatus = loadProgram();
    if (loadStatus)
      Serial.println(LOAD_STATUS_SUCCESS);
    else
      Serial.println(LOAD_STATUS_FAIL);
  }

private:
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
};
