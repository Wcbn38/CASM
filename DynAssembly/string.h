#pragma once

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

typedef int BOOL;
typedef char* string;

/*
Compares two c-strings.
Returns true if both strings are strictly identical
*/
BOOL strcmp(string str1, string str2);

/*
Returns the size of the c-string.
*/
int strsize(string str);

/*
Returns the number of caracter specified by the tag argument were present int the c-string.
*/
int strcount(string str, char tag);

/*
Copies a c-string from 'src' to 'dst'. The dst field needs to be allocated including the null terminator
*/
void strcopy(string src, string dst);

/*
Removes a specific caracter from the entire string.
*/
void strflush(string str, char tag);