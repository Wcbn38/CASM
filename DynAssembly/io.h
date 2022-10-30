#pragma once

#pragma once
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"

//OS SPECIFIC CONFIG MERGER
#ifdef LINUX
#include <unistd.h>
#endif

#ifdef WINDOWS
#include <io.h>
#include <sys/stat.h>
#endif

typedef int HANDLE;

/*
Reads the current line in the file.

Parameters
	- file: the file to be read
	- buf: pointer to receiving buffer
	- size: the size of the given buffer

Returs the number of bytes read or the following error codes (negative):
	- ERR_EOF: no more data is to be read.
	- ERR_READ_FAIL: failed read, handle may be corrupted.
	- ERR_OUT_OF_BOUNDS: buffer is not big enough
*/
int getline(HANDLE file, char* buf, int size);

/*
Fill the file with a specific character without discarding existing ones

Parameters
	- file: the file to be written to
	- length: the number of character to write
	- character: the caracter to fill with

Returs SUCCESS or the following error codes (negative):
	- ERR_READ_FAIL: failed read, handle may be corrupted.
	- ERR_WRITE_FAILED: failed read, handle may be corrupted.
*/
int fillFile(HANDLE file, int length, char character);