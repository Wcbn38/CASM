#include "io.h"

int getline(HANDLE file, char* buf, int size)
{
	int iResult;
	size--; //for null terminator space

	for (int i = 0; i < size; i++, buf++)
	{
		iResult = _read(file, buf, 1);

		switch (iResult)
		{
		case 1: 
			if (*buf == '\n')
			{
				*buf = '\0';
				return i;
			}
			break;

		case 0:
			*buf = '\0';
			if (i > 0)
				return i;
			else
				return ERR_EOF;
			break;

		default:
			return ERR_READ_FAIL;
			break;
		}
	}

	*buf = '\0';
	return ERR_OUT_OF_BOUNDS;
}

int fillFile(HANDLE file, int length, char character)
{
	char buf;
	int iResult;

	for (; length > 0 ; length--)
		if (iResult = _read(file, &buf, sizeof(buf)) < 0)
			return ERR_READ_FAIL;
		else if (iResult == 0) //EOF
			break;

	for (; length > 0; length--)
		if (_write(file, &character, 1) == -1)
			return ERR_WRITE_FAILED;

	return SUCCESS;
}