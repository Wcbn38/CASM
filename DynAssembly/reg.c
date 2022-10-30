#include "reg.h"

//private macros
#define ASSERT_MALLOC(a) if((a) == NULL) return

//public functions
void initRegisterSet(PREGISTER_SET set)
{
	set->first = NULL;
	set->last = set->first;
}

void freeRegisterSet(PREGISTER_SET set)
{
	REGISTER_CONTAINER* currNode; //node to delete
	REGISTER_CONTAINER* nextNode = set->first; //next node to delete

	while (nextNode != NULL)
	{
		currNode = nextNode;
		nextNode = currNode->next;
		free(currNode->reg.name);
		free(currNode);
	}

	set->first = NULL;
	set->last = NULL;
}

PREGISTER getRegister(REGISTER_SET set, const string name)
{
	while (set.first != NULL)
	{
		if (strcmp(set.first->reg.name, name))
		{
			return &set.first->reg;
		}

		set.first = set.first->next;
	}

	return NULL;
}

void newRegister(PREGISTER_SET set, PREGISTER inst)
{
	if (set->last != NULL)
	{
		set->last->next = (REGISTER_CONTAINER*) malloc(sizeof(REGISTER_CONTAINER));
		set->last = set->last->next;
	}
	else
		set->last = (REGISTER_CONTAINER*)malloc(sizeof(REGISTER_CONTAINER));

	ASSERT_MALLOC(set->last);

	set->last->reg.name = (string) malloc(strsize(inst->name) + 1);
	ASSERT_MALLOC(set->last->reg.name);

	strcopy(inst->name, set->last->reg.name);
	set->last->reg.size = inst->size;
	set->last->reg.value = inst->value;

	if (set->first == NULL)
		set->first = set->last;

	set->last->next = NULL;
}