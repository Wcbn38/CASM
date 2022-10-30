#include "lbl.h"

//private macros
#define FREE_CHECK(a) if((a) != NULL) free(a)
#define ASSERT_MALLOC(a) if((a) == NULL) return

//public functions
void initLabelSet(PLABEL_SET set)
{
	set->first = NULL;
	set->last = set->first;
}

void freeLabelSet(PLABEL_SET set)
{
	LABEL_CONTAINER* currNode;
	LABEL_CONTAINER* nextNode = set->first;

	while (nextNode != NULL)
	{
		currNode = nextNode;
		nextNode = currNode->next;
		free(currNode->lbl.name);
		free(currNode);
	}

	set->first = NULL;
	set->last = NULL;
}

PLABEL getLabel(LABEL_SET set, const string name)
{
	while (set.first != NULL)
	{
		if (strcmp(set.first->lbl.name, name))
			return &set.first->lbl;

		set.first = set.first->next;
	}

	return NULL;
}

void newLabel(PLABEL_SET set, PLABEL inst)
{
	if (set->last != NULL)
	{
		set->last->next = (LABEL_CONTAINER*) malloc(sizeof(LABEL_CONTAINER));
		set->last = set->last->next;
	}
	else
		set->last = (LABEL_CONTAINER*) malloc(sizeof(LABEL_CONTAINER));

	ASSERT_MALLOC(set->last);

	set->last->lbl.name = (string) malloc(strsize(inst->name) + 1);
	if (set->last->lbl.name == NULL)
		return;

	set->last->lbl.addr = inst->addr;
	strcopy(inst->name, set->last->lbl.name);

	if (set->first == NULL)
		set->first = set->last;

	set->last->next = NULL;
}

void freeLabel(PLABEL lbl)
{
	FREE_CHECK(lbl->name);
}