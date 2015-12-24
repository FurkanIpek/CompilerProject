#include "CodeGen.h"

t_compiler* compiler;

void usage()
{
	printf("\n\tUsage: ./pl4 input_file output_file\n\n");
}

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

	//disableLogging(1); // logs from codegen
	//disableLogging(2); // logs from tree

	compiler = initializeCodeGenerator(input, output);

	loadFile(compiler);

	if (parse(compiler) == 1)
		printf("Parsing finished succesfully.\n");

	freeCodeGenerator(compiler);
		
	return 0;
}

