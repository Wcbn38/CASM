#include "config.h"

int errorCodeVar = 0;
char* errorMsgVar = NULL;

int handleError(int errorCode, const char* msg)
{
	errorCodeVar = errorCode;
	
	if (msg != NULL)
	{
		errorMsgVar = (const char*) malloc(strsize(msg) + 1);
		if (errorMsgVar == NULL)
			errorCodeVar = ERR_INVALID_ERROR_HANDLING;
		else
			strcopy(msg, errorMsgVar);
	}
	else
	{
		if (errorMsgVar != NULL)
			free(errorMsgVar);

		errorMsgVar = NULL;
	}

	return errorCode == SUCCESS ? SUCCESS : -1;
}

const char* getErrorMsg()
{
	return errorMsgVar;
}

int getErrorCode()
{
	return errorCodeVar;
}