#include "parser.h"

BOOL getBlockingCondition(const char* format, char eval)
{
	if (*format == '%')
	{
		switch (*(format + 1))
		{
		case 's': //c-string
			return TRUE;
		case 'u': //unsigned integer
			return eval >= '0' && eval <= '9';
		case 'i': //signed integer
			return eval >= '0' && eval <= '9' || eval == '-';
		case 'c': //caracter
			return TRUE;
		case '*': //anything
			return TRUE;
		case 'b': //blank
			return (eval == ' ' || eval == '\t' || eval == '\n' || eval == '\r');

			//% caracter
		case '%':
		case '\0': //treat '%' as non-format
		default:
			return eval == '%';
		}
	}
	else
		return *format == eval;
}

BOOL parse(const char* str, const char* format, ...)
{
	va_list args = NULL;
	va_start(args, format);

	while (*str != '\0' && *format != '\0')
	{
		if (*format == '%')
		{
			//format has specifier
			switch (*(format + 1))
			{
			case 's': //c-string
			{
				char* extract = va_arg(args, char*);
				while (!getBlockingCondition(format + 2, *str) && *str != '\0')
				{
					*extract = *str;
					extract++; str++;
				}
				*extract = '\0';
				break;
			}
			case 'u': //unsigned integer
			{
				unsigned long* extract = va_arg(args, unsigned long*);
				*extract = strToInt(&str);
				break;
			}
			case 'i': //signed integer
			{
				signed long* extract = va_arg(args, signed long*);
				if (*str == '-')
				{
					str++;
					*extract = strToInt(&str) * -1;
				}
				else
				{
					*extract = strToInt(&str);
				}
				break;
			}
			case 'c': //caracter
			{
				char* extract = va_arg(args, char*);
				*extract = *str;
				str++;
				break;
			}

			//with no extract
			case '*': //anything
				while (!getBlockingCondition(format + 2, *str) && *str != '\0')
					str++;
				break;
			case 'b': //blank
				if ((*str != ' ' && *str != '\t' && *str != '\n' && *str != '\r'))
					return FALSE;
				while ((*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') && *str != '\0')
					str++;
				break;

				//special cases
			case '%':
				format++;
			case '\0': //treat '%' as non-format
			default: //treat as non-format
				if (*str != *format)
					return FALSE;

				str++; format--;
				break;
			}

			format += 2;
		}
		else
		{
			//format hasn't specifier
			if (*str != *format)
				return FALSE;

			str++; format++;
		}
	}

	va_end(args);

	return *str == *format || *(format) == '%' && *(format + 1) == '*' && *(format + 2) == '\0';
}

unsigned long strToInt(const char** str)
{
	unsigned long res = 0;

	if ((*str)[0] == '0')
	{
		if ((*str)[1] == 'X' || (*str)[1] == 'x')
		{
			(*str) += 2;
			while (TRUE)
			{
				if (**str >= '0' && **str <= '9')
				{
					res <<= 4;
					res += **str - '0';
				}
				else if (**str >= 'a' && **str <= 'f')
				{
					res <<= 4;
					res += **str - 'a' + 10;
				}
				else if (**str >= 'A' && **str <= 'F')
				{
					res <<= 4;
					res += **str - 'A' + 10;
				}
				else
					break;

				(*str)++;
			}

			return res;
		}
	}

	while (**str >= '0' && **str <= '9')
	{
		res *= 10;
		res += **str - '0';
		(*str)++;
	}

	return res;
}

long strToIntWSign(const char** str)
{
	switch (**str)
	{
	case '-':
		*str++;
		return strToInt(str) * -1;
	case '+':
		*str++;
		return strToInt(str);
	default:
		return strToInt(str);
	}
}

char** split(const char* str, const char* delim)
{
	char** extract;

	//get delimitor size
	int delimSize;
	for (delimSize = 0; delim[delimSize] != '\0'; delimSize++);

	//find the number of elements
	int nbElem = 0;

	{
		const char* strIterator = str;

		while (*strIterator != '\0')
		{
			if (cmp(strIterator, delim))
			{
				nbElem++;
				strIterator += delimSize;
			}
			else
				strIterator++;
		}

		if (strIterator != str) //number of tags + 1
			nbElem++;
	}

	//extract the elements
	{
		extract = (char**)malloc((nbElem + 1) * sizeof(char*));
		if (extract == NULL)
			return NULL;

		extract[nbElem] = NULL;

		const char* strIterator = str;

		for (int i = 0; i < (nbElem - 1); i++)
		{
			const char* strIteratorStart = strIterator;

			//get str size
			int strSize;
			for (strSize = 0; !cmp(strIterator, delim); strSize++, strIterator++);
			strIterator += delimSize;

			extract[i] = (char*)malloc(strSize * sizeof(char) + 1);
			if (extract[i] == NULL)
				return NULL;

			//copy str
			extract[i][strSize] = '\0';
			for (int j = 0; j < strSize; j++)
				extract[i][j] = strIteratorStart[j];
		}

		if (nbElem != 0)
		{
			const char* strIteratorStart = strIterator;

			//get str size
			int strSize;
			for (strSize = 0; *strIterator != '\0'; strSize++, strIterator++);
			strIterator += delimSize;

			extract[nbElem - 1] = (char*)malloc(strSize * sizeof(char) + 1);
			if (extract[nbElem - 1] == NULL)
				return NULL;

			//copy str
			extract[nbElem - 1][strSize] = '\0';
			for (int j = 0; j < strSize; j++)
				extract[nbElem - 1][j] = strIteratorStart[j];
		}
	}

	return extract;
}

void freeSplit(const char** splt)
{
	if (splt == NULL)
		return;

	for (int i = 0; splt[i] != NULL; i++)
		free(splt[i]);

	free(splt);
}

BOOL cmp(const char* src, const char* cmp)
{
	while (*src != '\0' && *cmp != '\0')
	{
		if (*src != *cmp)
			return FALSE;

		src++; cmp++;
	}

	if (*src == '\0' && *cmp != '\0')
		return FALSE;

	return TRUE;
}