#include "CodeGen.h"

#define PRINT // comment out before submitting

void debugInfo(char* str)
{
#ifdef PRINT
	printf("Debug: %s\n", str);
#endif
}

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
}

int parse(t_compiler* compiler)
{
    if (yyparse())
        return 0;

	return 1;
}

void genTemp(char* var)
{
	static int n = 0;
	char temp[10];
	snprintf(temp, 10, "%s%i", "temp", n++);
	strcpy(var, temp);
}

void genLabel(char* label)
{
	static int n = 0;
	char temp[10];
	snprintf(temp, 10, "%s%i", "label", n++);
	strcpy(label, temp);
}

void writeC(t_compiler* compiler, char* str, int newline)
{
	debugInfo(str);
	if (newline == 0)
		fprintf(compiler->fout, "%s", str);
	else
		fprintf(compiler->fout, "%s\n", str);
}

