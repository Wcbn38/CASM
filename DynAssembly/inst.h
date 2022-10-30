#pragma once
#include "string.h"
#include "utils.h"
#include "parser.h"
#include "config.h"

typedef struct
{
	int size; //in bits
	string name;
	int nb_ops;
	string op_ops; //types: &, \0, @, #
	string format;
} INSTRUCTION;

typedef struct INSTRUCTION_CONTAINER
{
	struct INSTRUCTION_CONTAINER* next;
	INSTRUCTION inst;
} INSTRUCTION_CONTAINER;

typedef struct
{
	INSTRUCTION_CONTAINER* first;
	INSTRUCTION_CONTAINER* last;
} INSTRUCTION_SET;

typedef INSTRUCTION* PINSTRUCTION;
typedef INSTRUCTION_SET* PINSTRUCTION_SET;

void initInstructionSet(PINSTRUCTION_SET set);

void freeInstructionSet(PINSTRUCTION_SET set);

PINSTRUCTION getInstruction(INSTRUCTION_SET set, const string name, int nb_op, string operands);

void newInstruction(PINSTRUCTION_SET set, PINSTRUCTION inst);

void freeInstruction(PINSTRUCTION inst);

uint64_t encodeInst(PINSTRUCTION inst, uint64_t* parameters);