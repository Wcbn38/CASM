#pragma once
#include "inst.h"
#include "mod.h"

typedef struct INST_QUEUE
{
	struct INST_QUEUE* next;
	addr addr;
	PINSTRUCTION inst;
	string* params;
	int nb_params;
} INST_QUEUE;

typedef struct
{
	INST_QUEUE* first;
	INST_QUEUE* last;
} INST_QUEUE_SET;

typedef INST_QUEUE* PINST_QUEUE;
typedef INST_QUEUE_SET* PINST_QUEUE_SET;

void initInstQueueSet(PINST_QUEUE_SET set);

void pushInstQueue(PINST_QUEUE_SET set, PINST_QUEUE qinst);

PINST_QUEUE popInstQueue(PINST_QUEUE_SET set);

void freeInstQueue(PINST_QUEUE qinst);