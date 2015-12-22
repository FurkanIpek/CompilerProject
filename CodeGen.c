#include "CodeGen.h"

const int log_id = 1;

t_compiler* initializeCodeGenerator(char* fin, char* fout)
{
	t_compiler* ptr = (t_compiler*) malloc(sizeof(t_compiler));
	ptr->fin_name = fin;
	ptr->fout_name = fout;

	return ptr;
}

void freeCodeGenerator(t_compiler* compiler)
{
	fclose(compiler->fin);
	fclose(compiler->fout);
	free(compiler);
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

void expression(char* operator, Node* base, Node* opr1, Node* opr2)
{
	char temp[10]; genTemp(temp);

	base = makeNode(strdup(temp), "", "", opr1, opr2);
	base->code = (char*) malloc(sizeof(char) * 256);

	snprintf(base->code, 256, "%s%s%s%s%s%s%s", opr1->code, opr2->code, base->var, " = ", opr1->var, operator, opr2->var);
}

void callStatement(char* procName)
{
	// TODO
}

void exprList(char* str, Node* node)
{
	
}

void writeC(t_compiler* compiler, char* str, int newline)
{
	debugInfo(str, log_id);
	if (newline == 0)
		fprintf(compiler->fout, "%s", str);
	else
		fprintf(compiler->fout, "%s\n", str);
}

