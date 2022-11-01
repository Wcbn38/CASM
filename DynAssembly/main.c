#include "main.h"

#ifndef DEBUG

int main(int argc, char** argv)
{
	//COMPILATION VARIABLES
	REGISTER_SET regSet;
	INSTRUCTION_SET instSet;
	CMP_PARAMETERS params;
	initRegisterSet(&regSet);
	initInstructionSet(&instSet);
	initParams(&params);

	//PARAMETERS DECODING
	LIST asmSetFiles;
	LIST outputFiles;
	LIST srcFiles;
	initList(&asmSetFiles);
	initList(&outputFiles);
	initList(&srcFiles);

	if (argc == 1)
	{
		printf("%s" HELP_MSG, argv[0]);
		return 0;
	}

	for (int i = 1; i < argc; i++)
	{
		if (cmp("-c", argv[i])) //config file
		{
			i++;
			if (addItem(&asmSetFiles, (void*) argv[i]) != 0)
			{
				return -1 * ERR_MEMORY_FAIL;
			}
		}
		else if (cmp("-o", argv[i])) //output file
		{
			i++;
			if (i >= argc)
			{
				printf("[ERROR] Parameter missing.\n");
				return -1 * ERR_OUT_OF_BOUNDS;
			}

			if (addItem(&outputFiles, (void*)argv[i]) != 0)
			{
				return -1 * ERR_MEMORY_FAIL;
			}
		}
		else if (cmp("-h", argv[i]))
		{
			printf("%s" HELP_MSG, argv[0]);
			return 0;
		}
		else
		{
			if (addItem(&srcFiles, (void*)argv[i]) != 0)
			{
				return -1 * ERR_MEMORY_FAIL;
			}
		}
	}

	//INSTRUCTION EXTRACTION
	{
		HANDLE* asmSetHandles = (HANDLE*) malloc(getSize(&asmSetFiles) * sizeof(HANDLE) + 1);
		if (asmSetHandles == NULL)
			return -1 * ERR_MEMORY_FAIL;

		int currIndex = 0;
		HANDLE asmFile;

		char* asmFileName = removeItem(&asmSetFiles, 0);
		
		while (asmFileName != NULL)
		{
			asmFile = _open(asmFileName, O_RDONLY);

			if (asmFile == -1)
			{
				printf("[WARNING] Failed to open instruction set file: %s\n", asmFileName);
			}
			else
			{
				printf("[INFO] Opened instruction set file: %s\n", asmFileName);
				asmSetHandles[currIndex] = asmFile;
				currIndex++;
			}

			asmFileName = removeItem(&asmSetFiles, 0);
		}

		asmSetHandles[currIndex] = -1;

		if (readInstructionSet(asmSetHandles, &instSet, &regSet) != SUCCESS)
		{
			if (getErrorMsg() != NULL)
				printf("[ERROR] %d: %s", getErrorCode(), getErrorMsg());
			else
				printf("[ERROR] %d", getErrorCode());

			return -1;
		}

		free(asmSetHandles);
	}

	//COMPILATION
	{
		//extract source files
		HANDLE* srcHandles = (HANDLE*)malloc(getSize(&srcFiles) * sizeof(HANDLE) + 1);
		if (srcHandles == NULL)
			return -1 * ERR_MEMORY_FAIL;

		{
			int currSrcFileIndex = 0;
			HANDLE srcFile;

			char* srcFileName = removeItem(&srcFiles, 0);

			while (srcFileName != NULL)
			{
				srcFile = _open(srcFileName, O_RDONLY);

				if (srcFile == -1)
				{
					printf("[WARNING] Failed to open source file: %s\n", srcFileName);
				}
				else
				{
					printf("[INFO] Opened source file: %s\n", srcFileName);
					srcHandles[currSrcFileIndex] = srcFile;
					currSrcFileIndex++;
				}

				srcFileName = removeItem(&srcFiles, 0);
			}

			srcHandles[currSrcFileIndex] = -1;
		}

		//extract output files
		HANDLE outFile;
		char* outFileNameRaw = removeItem(&outputFiles, 0);

		char outFileName[_MAX_PATH];
		FILE_PROPERTIES newFileProperties;

		while (outFileNameRaw != NULL)
		{
			newFileProperties.baseAddr = 0;
			newFileProperties.range = 0;

			if (parse(outFileNameRaw, "%s<%u:%u>", outFileName, &newFileProperties.baseAddr, &newFileProperties.range))
				newFileProperties.hasRange = TRUE;
			else if(parse(outFileNameRaw, "%s", outFileName))
				newFileProperties.hasRange = FALSE;

			outFile = _open(outFileName, O_CREAT | O_TRUNC | O_RDWR, _S_IREAD | _S_IWRITE);

			if (outFile == -1)
			{
				printf("[WARNING] Failed to open output file: %s\n", outFileName);
			}
			else
			{
				newFileProperties.fd = outFile;
				printf("[INFO] Opened output file: %s\n", outFileName);
				addFile(&params, &newFileProperties);
			}

			outFileNameRaw = removeItem(&outputFiles, 0);
		}

		if (compile(srcHandles, &instSet, &regSet, &params) != SUCCESS)
		{
			if (getErrorMsg() != NULL)
				printf("[ERROR] %d: %s", getErrorCode(), getErrorMsg());
			else
				printf("[ERROR] %d", getErrorCode());

			return -1;
		}

		free(srcHandles);
	}

	printf("[INFO] Successfully compiled.");
	return 0;
}

#endif