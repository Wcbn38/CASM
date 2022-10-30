#include "compilator.h"

#define DISCARD_BLANK_STR(a) strflush(a, ' '); strflush(a, '\t')

int readInstructionSet(HANDLE* files, PINSTRUCTION_SET instSet, PREGISTER_SET regSet)
{
	initInstructionSet(instSet);
	initRegisterSet(regSet);

	char rawLineBuffer[STR_SIZE];

	//memory buffers
	char rawDecodeBuffer1[STR_SIZE];
	char rawDecodeBuffer2[STR_SIZE];
	char rawDecodeBuffer3[STR_SIZE];

	int iResult;
	int opsize;

	while (*files != -1)
	{
		int line = 0;
		//get op size
		iResult = getline(*files, rawLineBuffer, sizeof(rawLineBuffer));
		line++;
		if (iResult < 0)
		{
			//end of file or error
			switch (iResult)
			{
			case ERR_EOF:
				return SUCCESS;
			case ERR_OUT_OF_BOUNDS:
				return ERR_NOT_ENOUGH_SPACE;
			case ERR_READ_FAIL:
				return ERR_READ_FAIL;
			default:
				return ERR_UNKNOWN;
			}
		}

		if (!parse(rawLineBuffer, "OP%u", &opsize))
			return handleError(ERR_INVALID_FORMAT, "Operators size not specified.");

		while (TRUE)
		{
			iResult = getline(*files, rawLineBuffer, sizeof(rawLineBuffer));
			line++;
			if (iResult < 0)
			{
				//end of file or error
				switch (iResult)
				{
				case ERR_EOF:
					return SUCCESS;
				case ERR_OUT_OF_BOUNDS:
					return handleError(ERR_NOT_ENOUGH_SPACE, "Buffer size isn't large enough.");
				case ERR_READ_FAIL:
					return handleError(ERR_READ_FAIL, "Failed to read line of file.");
				default:
					return handleError(ERR_UNKNOWN, "Failed to read line of file.");
				}
			}

			string commentBuffer = rawLineBuffer;
			while (*commentBuffer != '\0' && *commentBuffer != ';')
				commentBuffer++;

			*commentBuffer = '\0';

			//parse
			if (parse(rawLineBuffer, "%*=%*")) //register
			{
				REGISTER myRegister = {
					.size = opsize,
					.name = rawDecodeBuffer1
				};

				if (!parse(rawLineBuffer, "%s=%s", myRegister.name, rawDecodeBuffer2))
					return handleError(ERR_INVALID_FORMAT, "Register could not be parsed.");

				myRegister.value = 0;
				string valuePtr = rawDecodeBuffer2;
				while (*valuePtr != '\0')
				{
					if (*valuePtr == '0' || *valuePtr == '1')
						myRegister.value = (myRegister.value) << 1 | *valuePtr - '0';

					valuePtr++;
				}

				DISCARD_BLANK_STR(myRegister.name);
				newRegister(regSet, &myRegister);
			}
			else if (parse(rawLineBuffer, "%*(%*)%*")) //instruction
			{
				INSTRUCTION myInstruction = {
					.size = opsize,
					.name = rawDecodeBuffer1,
					.nb_ops = 0,
					.op_ops = rawDecodeBuffer2,
					.format = rawDecodeBuffer3
				};

				if(!parse(rawLineBuffer, "%s(%s)%s", myInstruction.name, myInstruction.op_ops, myInstruction.format))
					return handleError(ERR_INVALID_FORMAT, "Instruction could not be parsed.");

				DISCARD_BLANK_STR(myInstruction.name);
				string* opsSplit = split(myInstruction.op_ops, ",");

				while (opsSplit[myInstruction.nb_ops] != NULL)
				{
					DISCARD_BLANK_STR(opsSplit[myInstruction.nb_ops]);

					if (opsSplit[myInstruction.nb_ops][0] != '&' &&
						opsSplit[myInstruction.nb_ops][0] != '#' &&
						opsSplit[myInstruction.nb_ops][0] != '@')
					{
						myInstruction.op_ops[myInstruction.nb_ops] = '\0';
					}
					else
					{
						myInstruction.op_ops[myInstruction.nb_ops] = opsSplit[myInstruction.nb_ops][0];
					}
					myInstruction.nb_ops++;
				}

				newInstruction(instSet, &myInstruction);
			}
			else if (parse(rawLineBuffer, "%b") || parse(rawLineBuffer, ""))
			{
				//nothing to do
			}
			else
				return handleError(ERR_INVALID_FORMAT, "Failed to decode line.");
		}

		files++;
	}

	return SUCCESS;
}

int compile(HANDLE* inFiles, PINSTRUCTION_SET instSet, PREGISTER_SET regSet, PCMP_PARAMETERS cmpParams)
{
	//flush errors
	handleError(SUCCESS, NULL, 0);

	LABEL_SET lblSet;
	INST_QUEUE_SET instQueue;

	initLabelSet(&lblSet);
	initInstQueueSet(&instQueue);

	int iResult;
	char rawLineBuffer[STR_SIZE];
	char rawDecodeBuffer[STR_SIZE];

	while (*inFiles != -1)
	{
		int line = 0;
		iResult = getline(*inFiles, rawLineBuffer, sizeof(rawLineBuffer));
		if (iResult < 0)
		{
			//end of file or error
			switch (iResult)
			{
			case ERR_EOF:
				inFiles++;
				rawLineBuffer[0] = '\0';
				break;
			case ERR_OUT_OF_BOUNDS:
				return handleError(ERR_NOT_ENOUGH_SPACE, "Buffer size isn't large enough.");
			case ERR_READ_FAIL:
				return handleError(ERR_READ_FAIL, "Failed to read line of file.");
			default:
				return handleError(ERR_UNKNOWN, "Failed to read line of file.");
			}
		}

		string commentBuffer = rawLineBuffer;
		while (*commentBuffer != '\0' && *commentBuffer != ';')
			commentBuffer++;

		*commentBuffer = '\0';

		PFILE_PROPERTIES outFile = getCurrFile(cmpParams);
		if (outFile == NULL)
			return handleError(ERR_NO_CONTAINER, "Failed to find corresponding file for address.");

		while (parse(rawLineBuffer, "%s:%s", rawDecodeBuffer, rawLineBuffer))
		{
			LABEL lbl = {
				.name = rawDecodeBuffer,
				.addr = getCurrAddr(cmpParams)
			};

			strflush(rawLineBuffer, ' ');
			strflush(rawLineBuffer, '\t');

			newLabel(&lblSet, &lbl);
		}

		if (parse(rawLineBuffer, "%s:", rawDecodeBuffer))
		{
			LABEL lbl = {
				.name = rawDecodeBuffer,
				.addr = getCurrAddr(cmpParams)
			};

			strflush(rawDecodeBuffer, ' ');
			strflush(rawDecodeBuffer, '\t');

			newLabel(&lblSet, &lbl);
			rawLineBuffer[0] = '\0'; //end line
		}

		char opName[STR_SIZE];
		char opRawParams[STR_SIZE];

		string rawDecodeBufferBlankFlushed = rawLineBuffer;

		//flushes starting blank caracters
		while (*rawDecodeBufferBlankFlushed == ' ' || *rawDecodeBufferBlankFlushed == '\t')
			rawDecodeBufferBlankFlushed++;

		if (parse(rawDecodeBufferBlankFlushed, "SECTION%b%s", opRawParams))
		{
			if (opRawParams[0] == '\0')
				return handleError(ERR_INVALID_FORMAT, "Section address not specified");

			addr newAddr;

			if (opRawParams[0] >= '0' && opRawParams[0] <= '9')
			{
				char* opRawPtr = opRawParams;
				newAddr = strToInt(&opRawPtr);
			}
			else
			{
				PLABEL lbl = getLabel(lblSet, opRawParams);
				if (lbl == NULL)
					return handleError(ERR_NOT_FOUND, "Failed to find label definition for section change. Labels need to be specified before use.");

				newAddr = lbl->addr;
			}

			changeCurrAddr(cmpParams, newAddr);
			outFile = getCurrFile(cmpParams);
			if (outFile == NULL)
				return handleError(ERR_NO_CONTAINER, "Failed to find corresponding file for address.");

			iResult = fillFile(outFile->fd, getCurrAddr(cmpParams) - outFile->baseAddr - _lseek(outFile->fd, 0, SEEK_CUR), 0);
			if (iResult != SUCCESS)
				return handleError(iResult, "Failed to fill file with stub caracters.");
		}
		else if(parse(rawDecodeBufferBlankFlushed, "%s %s", opName, opRawParams))
		{
			string* params = split(opRawParams, ",");
			uint64_t* decodedParams = NULL;
			PINSTRUCTION inst = NULL;

			if (params[0] == NULL)
			{
				inst = getInstruction(*instSet, opName, 0, NULL);
			}
			else
			{
				strflush(params[0], ' ');
				strflush(params[0], '\t');

				if (params[0] != '\0') //at least 1 operand
				{
					//get operands type
					char opTypes[STR_SIZE];
					int opCount = 0;

					while (params[opCount] != NULL)
					{
						strflush(params[opCount], ' ');
						strflush(params[opCount], '\t');

						switch (params[opCount][0])
						{
						case '#': opTypes[opCount] = '#';
							break;
						case '&': opTypes[opCount] = '&';
							break;
						default:
							{
								PREGISTER foundReg = getRegister(*regSet, params[opCount]);
								if (foundReg != NULL)
									opTypes[opCount] = '@';
								else //label
									opTypes[opCount] = '&';
								break;
							}
						}

						opCount++;
					}

					inst = getInstruction(*instSet, opName, opCount, opTypes);

					if (inst == NULL)
						return handleError(ERR_INVALID_FORMAT, "Failed to find instruction");

					decodedParams = (uint64_t*) malloc(sizeof(uint64_t) * opCount);
					if (decodedParams == NULL)
						return handleError(ERR_MEMORY_FAIL, "Failed to allocate buffer.");

					for (int i = 0; i < opCount; i++)
					{
						switch ((params[i])[0])
						{
						default:
							{
								PREGISTER foundReg = getRegister(*regSet, params[i]);
								if (foundReg != NULL)
								{
									decodedParams[i] = foundReg->value;
								}
								else //label
								{
									PLABEL foundLabel = getLabel(lblSet, params[i]);
									if (foundLabel != NULL)
										decodedParams[i] = foundLabel->addr;
									else
									{
										INST_QUEUE queue = {
											.addr = getCurrAddr(cmpParams),
											.inst = inst,
											.params = params,
											.nb_params = opCount,
										};

										pushInstQueue(&instQueue, &queue);
										goto instruction_written;
									}
								}
								break;
							}
						case '#':
						case '&': 
							{
								string val = params[i] + 1;
								decodedParams[i] = strToIntWSign(&val);
								break;
							}
						
						}
					}
				}
				else
				{
					inst = getInstruction(*instSet, opName, 0, NULL);
				}
			}
			
			uint64_t encodedValue = encodeInst(inst, decodedParams);
			if (getErrorCode() != SUCCESS) //failed to encode
				return -1;

			if(params != NULL)
				freeSplit(params);

			if (_write(outFile->fd, &encodedValue, inst->size / 8) < 1)
				return handleError(ERR_WRITE_FAILED, "Failed to write tou output file.");

			instruction_written: //goto for when instruction is written

			if (decodedParams != NULL)
				free(decodedParams);

			incrementAddr(cmpParams, inst->size / 8);
		}
		else
		{
			strflush(rawLineBuffer, ' ');
			strflush(rawLineBuffer, '\t');

			if (rawLineBuffer[0] != '\0')
				return handleError(ERR_INVALID_FORMAT, "Synthax error.", line);
		}
	}

	PINST_QUEUE instPop = popInstQueue(&instQueue);

	while (instPop != NULL)
	{
		uint64_t* decodedParams;

		decodedParams = (uint64_t*)malloc(sizeof(uint64_t) * instPop->nb_params);
		if (decodedParams == NULL)
			return handleError(ERR_MEMORY_FAIL, "Failed to allocate buffer.");

		for (int i = 0; i < instPop->nb_params; i++)
		{
			switch ((instPop->params[i])[0])
			{
			default:
				{
					PREGISTER foundReg = getRegister(*regSet, instPop->params[i]);
					if (foundReg != NULL)
					{
						decodedParams[i] = foundReg->value;
					}
					else //label
					{
						PLABEL foundLabel = getLabel(lblSet, instPop->params[i]);
						if (foundLabel != NULL)
							decodedParams[i] = foundLabel->addr;
						else
						{
							return handleError(ERR_NOT_FOUND, "Failed to find operand."); //operand not found
						}
					}
					break;
				}
			case '#':
			case '&':
				{
					string val = instPop->params[i] + 1;
					decodedParams[i] = strToIntWSign(&val);
					break;
				}

			}
		}

		uint64_t encodedValue = encodeInst(instPop->inst, decodedParams);
		if (getErrorCode() != SUCCESS) //failed to encode
			return -1;

		if (instPop->params != NULL)
			freeSplit(instPop->params);

		changeCurrAddr(cmpParams, instPop->addr);
		PFILE_PROPERTIES outFile = getCurrFile(cmpParams);
		if (outFile == NULL)
			return handleError(ERR_NO_CONTAINER, "Failed to find corresponding file for address.");

		if (outFile == NULL)
			return handleError(ERR_NO_CONTAINER, "Failed to find corresponding file for address");

		if (_lseek(outFile->fd, getCurrAddr(cmpParams) - outFile->baseAddr, SEEK_SET) == -1L)
			return handleError(ERR_WRITE_FAILED, "Failed to change file offset.");

		if (_write(outFile->fd, &encodedValue, instPop->inst->size / 8) < 1)
			return handleError(ERR_WRITE_FAILED, "Failed to write tou output file.");

		if (decodedParams != NULL)
			free(decodedParams);

		freeInstQueue(instPop);
		instPop = popInstQueue(&instQueue);
	}

	return SUCCESS;
}