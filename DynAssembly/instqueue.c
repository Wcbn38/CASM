#pragma once
#include "instqueue.h"

//private macros
#define FREE_CHECK(a) if((a) != NULL) free(a)
#define ASSERT_MALLOC(a) if((a) == NULL) return

//public functions
void initInstQueueSet(PINST_QUEUE_SET set)
{
	set->first = NULL;
	set->last = set->first;
}

void pushInstQueue(PINST_QUEUE_SET set, PINST_QUEUE qinst)
{
	if (set->last != NULL)
	{
		set->last->next = (PINST_QUEUE*)malloc(sizeof(PINST_QUEUE));
		set->last = set->last->next;
	}
	else
		set->last = (PINST_QUEUE*)malloc(sizeof(PINST_QUEUE));

	ASSERT_MALLOC(set->last);

	set->last->inst = qinst->inst;
	set->last->nb_params = qinst->nb_params;
	set->last->params = qinst->params;
	set->last->addr = qinst->addr;

	if (set->first == NULL)
		set->first = set->last;

	set->last->next = NULL;
}

PINST_QUEUE popInstQueue(PINST_QUEUE_SET set)
{
	PINST_QUEUE qinst = NULL;

	if (set->first != NULL)
	{
		qinst = set->first;
		set->first = set->first->next;
	}

	return qinst;
}

void freeInstQueue(PINST_QUEUE qinst)
{
	free(qinst);
}