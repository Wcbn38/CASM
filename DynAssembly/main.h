#pragma once
#include "compilator.h"
#include "io.h"
#include "config.h"
#include <stdio.h>
#include "list.h"

#define HELP_MSG	" [-c <config file>] [-o <output file>] [-h] [source files]\n" \
					"\n" \
					"Compiles assembly source files to the outputs files with config files as instruction sets.\n" \
					"\n" \
					"Parameters:\n" \
					"\t-c : config files corresponding to the instruction set used.\n" \
					"\t-o : output files to write the compilation results. To specify the address ranges, the format should be: PATH<BASE_ADDRESS:RANGE>. This format is optionnal, only the path can be specified. The order is important as the first files override the next one if addresses ranges are overlapping.\n" \
					"\t-h : prints this message.\n" \
					"\n" \
					"Example:\n" \
					"\tcasm -c config.asc -o out.exe<0x1000:0x1000> -o rest.exe main.asm\n" \
					"\n"