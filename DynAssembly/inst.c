#include "inst.h"

//private macros
#define FREE_CHECK(a) if((a) != NULL) free(a)
#define ASSERT_MALLOC(a) if((a) == NULL) return

//public functions
void initInstructionSet(PINSTRUCTION_SET set)
{
	set->first = NULL;
	set->last = set->first;
}

void freeInstructionSet(PINSTRUCTION_SET set)
{
	INSTRUCTION_CONTAINER* currNode; //node to delete
	INSTRUCTION_CONTAINER* nextNode = set->first; //next node to delete

	while (nextNode != NULL)
	{
		currNode = nextNode;
		nextNode = currNode->next;
		freeInstruction(&currNode->inst);
		free(currNode);
	}

	set->first = NULL;
	set->last = NULL;
}

PINSTRUCTION getInstruction(INSTRUCTION_SET set, const string name, int nb_op, string operands)
{
	while (set.first != NULL)
	{
		if (strcmp(set.first->inst.name, name)
			&& set.first->inst.nb_ops == nb_op)
		{
			//check if operands found
			for (int i = 0; i < set.first->inst.nb_ops; i++)
			{
				if (set.first->inst.op_ops[i] != operands[i])
					break; //no compatible operand found
			}

			return &set.first->inst;
		}

		set.first = set.first->next;
	}

	return NULL;
}

void newInstruction(PINSTRUCTION_SET set, PINSTRUCTION inst)
{
	if (set->last != NULL)
	{
		set->last->next = (INSTRUCTION_CONTAINER*) malloc(sizeof(INSTRUCTION_CONTAINER));
		set->last = set->last->next;
	}
	else
		set->last = (INSTRUCTION_CONTAINER*) malloc(sizeof(INSTRUCTION_CONTAINER));

	ASSERT_MALLOC(set->last);

	set->last->inst.name = (string) malloc(strsize(inst->name) + 1);
	set->last->inst.format = (string) malloc(strsize(inst->format) + 1);
	set->last->inst.op_ops = (string) malloc(inst->nb_ops);

	ASSERT_MALLOC(set->last->inst.name);
	ASSERT_MALLOC(set->last->inst.format);
	ASSERT_MALLOC(set->last->inst.op_ops);

	set->last->inst.size = inst->size;
	set->last->inst.nb_ops = inst->nb_ops;

	strcopy(inst->format, set->last->inst.format);
	strcopy(inst->name, set->last->inst.name);

	for (int i = 0; i < inst->nb_ops; i++)
		set->last->inst.op_ops[i] = inst->op_ops[i];

	if (set->first == NULL)
		set->first = set->last;

	set->last->next = NULL;
}

void freeInstruction(PINSTRUCTION inst)
{
	FREE_CHECK(inst->name);
	FREE_CHECK(inst->format);
	FREE_CHECK(inst->op_ops);
}

uint64_t encodeInst(PINSTRUCTION inst, uint64_t* parameters)
{
	uint64_t code = 0;
	string format = inst->format;

	while (*format != '\0')
	{
		switch (*format)
		{
		case '1':
			code = (code << 1) | 1;
			format++;
			break;
		case '0':
			code = (code << 1) | 0;
			format++;
			break;
		default:
			if (*format >= 'a' && *format <= 'z')
			{
				char paramNb;
				unsigned long length, offset = 0;

				if (parse(format, "%c'%u%*", &paramNb, &length)) {}
				else if(parse(format, "%c'%u'%u%*", &paramNb, &length)) {}
				else
				{
					return handleError(ERR_INVALID_FORMAT, "Failed to parse format when encoding instruction.");
				}

				paramNb -= 'a';
				if (paramNb >= inst->nb_ops)
				{
					return handleError(ERR_INVALID_FORMAT, "Failed to parse format when encoding instruction.");
				}

				uint64_t mask = 0;
				for (int i = 0; i < length; i++)
					mask = (mask << 1) | 0x01;

				mask <<= offset;

				code = (code << length) | (parameters[paramNb] & mask);

				while (*format != ' ' && *format != '\t' && *format != '\0') //wait for blank caracter
					format++;
			}
			else
			{
				format++;
			}
			break;
		}
	}

	return code;
}
