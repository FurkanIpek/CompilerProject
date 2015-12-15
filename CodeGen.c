#include "CodeGen.h"

void usage()
{
	printf("\n Usage: ./parser input_file output_file \n\n");
}

void freeCompiler(t_compiler* compiler)
{
	fclose(compiler->fin);
	fclose(compiler->fout);
	free(compiler);
}

t_compiler* initializeCodeGenerator(char* fin, char* fout)
{
	t_compiler* ptr = (t_compiler*) malloc(sizeof(t_compiler));
	ptr->fin_name = fin;
	ptr->fout_name = fout;

	return ptr;
}

void loadFile(t_compiler* compiler)
{
	compiler->fin = fopen(compiler->fin_name, "r");
	compiler->fout = fopen(compiler->fout_name, "w");
	
	yyset_in(compiler->fin);
	yyset_out(compiler->fout);
}

int parse(t_compiler* compiler)
{
	printf("Parsing..\n");
 
    if (yyparse()) {
        // error parsing
        return 0;
    }

	return 1;
}

int genTemp()
{
	static int n = 0;

	return n++;
}

void writeC(t_compiler* compiler, char* str)
{
	printf("Debug: %s \n", str);
	fprintf(compiler->fout, "%s", str);
}
