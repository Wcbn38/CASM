#pragma once
//config macro

#include "io.h"
#include "config.h"
#include "inst.h"
#include "instqueue.h"
#include "reg.h"
#include "string.h"
#include "lbl.h"

/*
The terminator value needs to be -1
*/
int readInstructionSet(HANDLE* files, PINSTRUCTION_SET instSet, PREGISTER_SET regSet);

/*
The terminator value needs to be -1
*/
int compile(HANDLE* inFiles, PINSTRUCTION_SET instSet, PREGISTER_SET regSet, PCMP_PARAMETERS outFile);