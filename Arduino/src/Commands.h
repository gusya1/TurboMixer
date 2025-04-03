#pragma once

#include <stdint.h>

enum class CommandType : uint8_t
{
    Mix = 0,
    Gap = 1,
    Alarm = 2
};

struct __attribute__((packed)) MixCommand
{
    CommandType commandType = CommandType::Mix;
    uint8_t argsCount = 0;
    int32_t power = 0;
    int32_t duration = 0;
};

struct __attribute__((packed)) GapCommand
{
    CommandType commandType = CommandType::Gap;
    uint8_t argsCount = 0;
    int32_t duration = 0;
};

struct __attribute__((packed)) AlarmCommand
{
    CommandType commandType = CommandType::Alarm;
    uint8_t argsCount = 0;
};