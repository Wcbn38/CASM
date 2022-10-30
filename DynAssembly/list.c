#include "list.h"

void initList(PLIST list)
{
	list->first = NULL;
	list->last = NULL;
	list->size = 0;
}

int addItem(PLIST list, void* data)
{
	if (list->last == NULL)
	{
		list->first = (LIST_SECTION*) malloc(sizeof(LIST_SECTION));
		list->last = list->first;

		if (list->last == NULL)
			return -1;
	}
	else
	{
		LIST_SECTION* newSection = (LIST_SECTION*) malloc(sizeof(LIST_SECTION));

		if (newSection == NULL)
			return -1;

		list->last->next = newSection;
		list->last = newSection;
	}

	list->last->next = NULL;
	list->last->data = data;

	list->size++;

	return 0;
}

void* removeItem(PLIST list, unsigned int index)
{
	LIST_SECTION* currSection = list->first;

	if (currSection == NULL)
		return NULL;

	while (index > 1 && currSection->next != NULL)
	{
		currSection = currSection->next;
		index--;
	}

	if (currSection->next == NULL && index == 0) //absolute index is -1
	{
		void* deleteData = currSection->data;
		initList(list);

		free(currSection);

		return deleteData;
	}

	if (currSection->next == NULL)
		return NULL;

	LIST_SECTION* toDelete = currSection->next;
	void* deleteData = toDelete->data;
	currSection->next = currSection->next->next;

	list->size--;

	free(toDelete);

	return deleteData;
}

void* getItem(PLIST list, unsigned int index)
{
	LIST_SECTION* currSection = list->first;

	while (index > 0 && currSection != NULL)
	{
		currSection = currSection->next;
		index--;
	}

	if (currSection == NULL)
		return NULL;
	else
		return currSection->data;
}

unsigned int getSize(PLIST list)
{
	return list->size;
}