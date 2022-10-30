#include "mod.h"

void initParams(PCMP_PARAMETERS params)
{
	params->currAddr = 0x0;
	params->files.nbFiles = 0;
	params->files.files = NULL;
}

void changeCurrAddr(PCMP_PARAMETERS params, addr newAddr)
{
	params->currAddr = newAddr;
}

FILE_PROPERTIES* getCurrFile(PCMP_PARAMETERS params)
{
	for (int i = 0; i < params->files.nbFiles; i++)
	{
		if (params->currAddr >= params->files.files[i]->baseAddr 
			&& (params->currAddr < (params->files.files[i]->baseAddr + params->files.files[i]->range) || !params->files.files[i]->hasRange)
			)
			return params->files.files[i];
	}

	return NULL;
}

addr getCurrAddr(PCMP_PARAMETERS params)
{
	return params->currAddr;
}

PFILE_PROPERTIES addFile(PCMP_PARAMETERS params, PFILE_PROPERTIES file)
{
	FILE_PROPERTIES** newArray = (FILE_PROPERTIES**)malloc((params->files.nbFiles + 1) * sizeof(FILE_PROPERTIES*));
	if (newArray == NULL)
		return;

	for (int i = 0; i < params->files.nbFiles; i++)
		newArray[i] = params->files.files[i];

	newArray[params->files.nbFiles] = (FILE_PROPERTIES*) malloc(sizeof(FILE_PROPERTIES));
	if (newArray[params->files.nbFiles] == NULL)
		return;

	*newArray[params->files.nbFiles] = *file;

	if(params->files.files != NULL)
		free(params->files.files);

	params->files.files = newArray;
	params->files.nbFiles++;

	return params->files.files[params->files.nbFiles - 1];
}

PFILE_PROPERTIES getFile(PCMP_PARAMETERS params, uint16_t index)
{
	if (index >= params->files.nbFiles)
		return NULL;

	return params->files.files[index];
}

void incrementAddr(PCMP_PARAMETERS params, int add)
{
	params->currAddr += add;
}