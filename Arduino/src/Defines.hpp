#pragma once

#define BUFFER_LEN 256

#define STX 0x02
#define ETX 0x03

#define LOAD_STATUS_READY     "READY"
#define LOAD_STATUS_LOADING   "LOADING"
#define LOAD_STATUS_SUCCESS   "SUCCESS"
#define LOAD_STATUS_FAIL      "FAIL"

// коды возврата
#define SUCCESS                 0
#define PROGRAM_FINISHED        1 // TODO remove
#define PROCESS_FINISHED        1
#define PROGRAM_READ_ERROR      -1
#define INCORRECT_POWER_ERROR   -2
#define LOADING_NOT_FINISHED    -3

#define LONG_PRESS_BUTTON_DURATION_SEC 3

#define INDICATOR_DIGIT_SWITCH_DURATION_MSEC 10

#define PIN_BUTTON      3
#define PIN_IND_SER     13
#define PIN_IND_RCLK    12
#define PIN_IND_SRCLK   11
#define PIN_IND_DIGIT_SWITCH 10