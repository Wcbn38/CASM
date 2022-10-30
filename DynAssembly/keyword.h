#pragma once
#include "utils.h"
#include "string.h"

enum TYPE
{
	TYPE_LABEL,
	TYPE_REGISTER,
	TYPE_INT,
	TYPE_STRING
};

typedef struct
{
	struct KEYWORD* next;
	string name;
	enum TYPE type;
	void* data;
} KEYWORD;

typedef struct {
	KEYWORD* first;
	KEYWORD* last;
} KEYWORD_SET;

typedef KEYWORD_SET* PKEYWORD_SET;
typedef KEYWORD* PKEYWORD;

void initKeywordSet(PKEYWORD_SET set);

void freeKeywordSet(PKEYWORD_SET set);

PKEYWORD getKeyword(KEYWORD_SET set, const string name);

void newKeyword(PKEYWORD_SET set, PKEYWORD inst);