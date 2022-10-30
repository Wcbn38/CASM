#pragma once
#include "string.h"
#include "mod.h"
#include "keyword.h"
#include "inst.h"

typedef struct
{
	string name;
	addr addr;
} LABEL;

typedef struct
{
	struct LABEL_CONTAINER* next;
	LABEL lbl;
} LABEL_CONTAINER;

typedef struct {
	LABEL_CONTAINER* first;
	LABEL_CONTAINER* last;
} LABEL_SET;

typedef LABEL* PLABEL;
typedef LABEL_SET* PLABEL_SET;

void initLabelSet(PLABEL_SET set);

void freeLabelSet(PLABEL_SET set);

PLABEL getLabel(LABEL_SET set, const string name);

void newLabel(PLABEL_SET set, PLABEL inst);

void freeLabel(PLABEL lbl);