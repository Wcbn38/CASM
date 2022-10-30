#pragma once
#include "utils.h"
#include "string.h"
#include <stdlib.h>

typedef struct
{
	int size; //int bits
	string name;
	uint64_t value;
} REGISTER;

typedef struct REGISTER_CONTAINER
{
	struct REGISTER_CONTAINER* next;
	REGISTER reg;
} REGISTER_CONTAINER;

typedef struct
{
	REGISTER_CONTAINER* first;
	REGISTER_CONTAINER* last;
} REGISTER_SET;

typedef REGISTER_SET* PREGISTER_SET;
typedef REGISTER* PREGISTER;

void initRegisterSet(PREGISTER_SET set);

void freeRegisterSet(PREGISTER_SET set);

PREGISTER getRegister(REGISTER_SET set, const string name);

void newRegister(PREGISTER_SET set, PREGISTER inst);