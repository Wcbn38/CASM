# CASM: Configurable Assembly

## What is CASM?

This project consists in creating a assembly compiler with the instruction not included in the compiler. This has the advantages for processor prototyping. The official project sources are available on [github](https://github.com/Wcbn38/CASM)

## How do I use it?

The help message can be accessed with the -h option. The message is the following:

```text
[-c <config file>] [-o <output file>] [-h] [source files]

Compiles assembly source files to the outputs files with config files as instruction sets.

Parameters:
	-c : config files corresponding to the instruction set used.
	-o : output files to write the compilation results. To specify the address ranges, the format should be: PATH<BASE_ADDRESS:RANGE>. 
		This format is optionnal, only the path can be specified. 
		The order is important as the first files override the next one if addresses ranges are overlapping.
	-h : prints this message.

Example:
	casm -c config.asc -o out.exe<0x1000:0x1000> -o rest.exe main.asm
```

### What are (assembly) config files?

These files contain the coding of each available instruction. One in instruction name can have multiple data access.

Each config file starts with `OP8`, were 8 needs to be replaced with the number of bits of each instruction. This is necessarly at the start of the file and cannot be changed for this file.

We then define the registers, the format is: `R1=0000 0000 0000 0001` were the register name is `R1` and it's binary value `0000 0000 0000 0001`.

Then, the instruction are encoded with the following format `INST(&a,#b) 01 a'3 b'3'2`. Lets check each part of the instruction:
- `INST` is the name of the instruction
- `(&a,#b)` are the operands. The parentheses are not mandatory! The `&` in `&a` is the access type. 3 access type are available:
    - `#` is for constants
	- `&` is for addresses including labels (wich are all considered as addresses)
	- `@` is for registers
	Only one access type may set for each operands.
- `01 a'3 b'3'2` is equal to saying that the result number will be (in c language): `res = (01 << 6) | ((a & 0x07) << 3) | ((b << 2) & 0x07)` wich creates a 8bit word.

### Why des output files can have specified addresse ranges?

This functionnality redirects the address written to to a specific file. It makes the possibility to the developper to seperate easily files corresponding to memory, files of code stack etc...

The synthax is `file.bin<0x1000:0x2000>` were:
- `file.bin` is the file name
- `0x1000` is the base address of wich the result will be written to.
- `0x2000` the range of addresse to which the result will be writtent to.

### What is the synthax for the source code?

Exemples will be added soon. The main point to note is the `SECTION` keyword that offsets the current compilation address.