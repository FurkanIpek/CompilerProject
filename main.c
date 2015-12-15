#include "CodeGen.h"

t_compiler* compiler;

int main(int argc, char** argv)
{
	char* input = "input.pl4", * output = "output.tac";
	
	if (argc >= 3)
	{
		input = argv[1];
		output = argv[2];
	}
	else
		usage();

	compiler = initializeCodeGenerator(input, output);

	loadFile(compiler);

	if (parse(compiler) == 1)
		printf("Parsing finished succesfully.\n");

	freeCompiler(compiler);
		
	return 0;
}
