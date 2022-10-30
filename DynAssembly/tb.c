#include "main.h"

#ifdef DEBUG

int main(int argc, char** argv)
{
	HANDLE files[] = { -1, -1 };
	INSTRUCTION_SET instSet;
	REGISTER_SET regSet;

	initInstructionSet(&instSet);
	initRegisterSet(&regSet);

	files[0] = _open("cfg_test_file.asc", O_RDONLY);
	if (files[0] == -1)
		return -1;

	if (readInstructionSet(files, &instSet, &regSet))
	{
		if (getErrorMsg() != NULL)
			printf("ERROR %d: %s", getErrorCode(), getErrorMsg());
		else
			printf("ERROR %d", getErrorCode());

		return -1;
	}

	_close(files[0]);

	files[0] = _open("test.asm", O_RDONLY);
	if (files[0] == -1)
		return -1;

	CMP_PARAMETERS compilationParams;
	FILE_PROPERTIES outFile;

	initParams(&compilationParams);
	outFile.fd = _open("cmp.out", O_CREAT | O_TRUNC | O_RDWR);
	if (outFile.fd == -1)
		return -1;
	outFile.baseAddr = 0x00;
	outFile.hasRange = FALSE;

	addFile(&compilationParams, &outFile);

	if (compile(files, &instSet, &regSet, &compilationParams))
	{
		if (getErrorMsg() != NULL)
			printf("ERROR %d: %s", getErrorCode(), getErrorMsg());
		else
			printf("ERROR %d", getErrorCode());

		return -1;
	}

	_close(files[0]);
	_close(outFile.fd);

	printf("SUCCESS");

	return 0;
}

#endif