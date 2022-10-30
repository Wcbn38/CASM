#pragma once

#include <stdarg.h>
#include <stdlib.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

typedef int BOOL;

//PROTOTYPES

/*
Parses if possible the format and extracts the different variables to the inputs.

Parameters
	- str: the string to extract the format
	- format: format representing the parsing. Formats are:
	- %s : c-string (needs to be allocated)
	- %u : unsigned integer (long)
	- %i : signed integer (long)
	- %% : caracter '%' (no variable needed)
	- %c : caracter
	- %* : anything (no variable needed)
	- %b : blank - at least one caracter spaces, tab or newline caracter (no variable needed)

Returns true if parsing was successfull. False instead.
*/
BOOL parse(const char* str, const char* format, ...);

/*
Converts a string to an unsigned integer. Handles decimal and hexadecimal (if 0x is added as prefix).
The hexadecimal conversion isn't case sensistive.
*/
unsigned long strToInt(const char** str);

/*
Converts a string to an unsigned integer. Handles decimal and hexadecimal (if 0x is added as prefix).
The hexadecimal conversion isn't case sensistive. Negative values are considered.
*/
long strToIntWSign(const char** str);

/*
Parses c-string with the format specified. The c-string generated is allocated with malloc, the user needs to free the memory.
The end of extraction is indicated by a NULL pointer.
*/
char** split(const char* str, const char* delim);

/*
Free the memory allocated by the split() function.
*/
void freeSplit(const char** splt);

/*
Compares two string, of with the src string needs to equal cmp. The src string does not need to have the same length as cmp.
*/
BOOL cmp(const char* src, const char* cmp);