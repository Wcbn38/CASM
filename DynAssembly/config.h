#pragma once
#include "utils.h"
#include "string.h"
#include <stdlib.h>

//PARAMETERS
#define STR_SIZE 256

//ERROR CODES
#define SUCCESS 0
#define ERR_SUCCESS SUCCESS

#define ERR_UNKNOWN -1
#define ERR_OPEN_FILE -2
#define ERR_MEMORY_FAIL -3
#define ERR_OUT_OF_BOUNDS -4
#define ERR_READ_FAIL -5
#define ERR_EOF -6
#define ERR_INVALID_FORMAT -7
#define ERR_NOT_ENOUGH_SPACE -8
#define ERR_WRITE_FAILED -9
#define ERR_NOT_FOUND -10
#define ERR_NO_CONTAINER -11

#define ERR_INVALID_ERROR_HANDLING -32768

//error handling

int handleError(int errorCode, const char* msg);

const char* getErrorMsg();

int getErrorCode();