#pragma once
#include <stdlib.h>

#ifndef NULL
#define NULL 0
#endif

typedef struct LIST_SECTION
{
	struct LIST_SECTION* next;
	void* data;
} LIST_SECTION;

typedef struct
{
	LIST_SECTION* first;
	LIST_SECTION* last;
	unsigned int size;
} LIST;

typedef LIST* PLIST;

void initList(PLIST list);

int addItem(PLIST list, void* data);

void* removeItem(PLIST list, unsigned int index);

void* getItem(PLIST list, unsigned int index);

unsigned int getSize(PLIST list);