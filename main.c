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
		
		compiler = initializeCodeGenerator(input, output);

		loadFile(compiler);

		if (parse(compiler) == 1)
		{
			printf("Intermediate code generation finished succesfully.\n");
			freeCodeGenerator(compiler);
			
			return EXIT_SUCCESS;
		}

		freeCodeGenerator(compiler);
	}
	else {
		usage();
	}
		
	return EXIT_FAILURE;
}

