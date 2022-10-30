#pragma once
#include "io.h"
#include "utils.h"
#include "keyword.h"

typedef uint64_t addr;

typedef struct
{
	HANDLE fd;
	addr baseAddr;
	addr range;
	BOOL hasRange;
} FILE_PROPERTIES;

typedef struct
{
	addr currAddr;
	struct {
		FILE_PROPERTIES** files;
		uint16_t nbFiles;
	} files;
} CMP_PARAMETERS;

typedef FILE_PROPERTIES* PFILE_PROPERTIES;
typedef CMP_PARAMETERS* PCMP_PARAMETERS;

void initParams(PCMP_PARAMETERS params);

void changeCurrAddr(PCMP_PARAMETERS params, addr newAddr);

addr getCurrAddr(PCMP_PARAMETERS params);

FILE_PROPERTIES* getCurrFile(PCMP_PARAMETERS params);

PFILE_PROPERTIES addFile(PCMP_PARAMETERS params, PFILE_PROPERTIES file);

PFILE_PROPERTIES getFile(PCMP_PARAMETERS params, uint16_t index);

void incrementAddr(PCMP_PARAMETERS params, int add);