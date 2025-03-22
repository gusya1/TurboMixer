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
#define RUNNING                 2
#define PROGRAM_READ_ERROR      -1
#define INCORRECT_POWER_ERROR   -2
#define LOADING_NOT_FINISHED    -3

#define LONG_PRESS_BUTTON_DURATION 3

#define PIN_BUTTON 3