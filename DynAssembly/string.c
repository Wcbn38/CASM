#include "string.h"

BOOL strcmp(string str1, string str2)
{
	while (*str1 != '\0' && *str2 != '\0')
	{
		if (*str1 != *str2)
			return FALSE;

		str1++; str2++;
	}

	if (*str1 == '\0' && *str2 == '\0')
		return TRUE;

	return FALSE;
}

int strsize(string str)
{
	int i = 0;

	while (str[i] != '\0')
		i++;

	return i;
}

int strcount(string str, char tag)
{
	int count;

	for (count = 0; *str != '\0'; str++)
	{
		if (*str == tag)
			count++;
	}

	return count;
}

void strcopy(string src, string dst)
{
	for (; *src != '\0'; src++, dst++)
		*dst = *src;

	*dst = '\0';
}

void strflush(string str, char tag)
{
	for (string fwdptr = str; *fwdptr != '\0'; fwdptr++)
		if (*fwdptr != tag)
		{
			*str = *fwdptr;
			str++;
		}

	*str = '\0';
}