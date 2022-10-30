#include "keyword.h"

//private macros
#define ASSERT_MALLOC(a) if((a) == NULL) return

//public functions
void initKeywordSet(PKEYWORD_SET set)
{
	set->first = NULL;
	set->last = set->first;
}

void freeKeywordSet(PKEYWORD_SET set)
{
	KEYWORD* currNode; //node to delete
	KEYWORD* nextNode = set->first; //next node to delete

	while (nextNode != NULL)
	{
		currNode = nextNode;
		nextNode = currNode->next;
		free(currNode->name);
		free(currNode);
	}

	set->first = NULL;
	set->last = NULL;
}

PKEYWORD getKeyword(KEYWORD_SET set, const string name)
{
	while (set.first != NULL)
	{
		if (strcmp(set.first->name, name))
			return set.first;

		set.first = set.first->next;
	}

	return NULL;
}

void newKeyword(PKEYWORD_SET set, PKEYWORD inst)
{
	if (set->last != NULL)
	{
		set->last->next = (KEYWORD*)malloc(sizeof(KEYWORD));
		set->last = set->last->next;
	}
	else
		set->last = (KEYWORD*)malloc(sizeof(KEYWORD));

	ASSERT_MALLOC(set->last);

	set->last->name = (string) malloc(strsize(inst->name) + 1);
	ASSERT_MALLOC(set->last->name);

	strcopy(inst->name, set->last->name);

	set->last->data = inst->data;
	set->last->type = inst->type;


	if (set->first == NULL)
		set->first = set->last;

	set->last->next = NULL;
}