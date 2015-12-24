#include "CodeGen.h"

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
	char temp [10];
	snprintf(temp, 10, "%s%i", "temp", n++);
	strcpy(var, temp);
}

void genLabel(char* label)
{
	static int n = 0;
	char temp [10];
	snprintf(temp, 10, "%s%i", "label", n++);
	strcpy(label, temp);
}

Node* genLeaf(char* var, char* label, char* code)
{
	return makeLeaf(var, label, code);
}

Node* expression(char* operator, Node* opr1, Node* opr2)
{
	char temp [10];
	char code [256];

	genTemp(temp);
	
	//if (strcmp(opr1->code, "") != 0) strcat(opr1->code, "\n")
	if (opr2->code != "") strcat(opr2->code, "\n");

	snprintf(code, 256, "%s%s%s%s%s%s%s", opr1->code, opr2->code, temp, " = ", opr1->var, operator, opr2->var);

	return makeNode(temp, operator, code, opr1, opr2);
}

Node* callStatement(char* procName, Node* exprList)
{
	return makeProcedureCall(procName, exprList);
}

void extendBranch(Node* branch, Node* statement, Node* bag)
{
	addToBranch(branch, statement, bag);
}

Node* whileStatement(Node* expr, Node* statements)
{
	char label1 [10]; genLabel(label1);
	char label2 [10]; genLabel(label2);

	return makeWhileNode(label1, label2, expr, statements);
}

Node* ifStatement(Node* expr, Node* ifBody, Node* elseBody)
{
	return makeIfElseNode(expr, ifBody, elseBody);
}

Node* assignStatement(char* var, Node* expr)
{
	char code [256];

	//printf("%s\n", expr->code);
	if (expr->code != "") strcat(expr->code, "\n");

	snprintf(code, 256, "%s%s%s%s", expr->code, var, " = ", expr->var);

	return makeNode(var, "=", code, expr, NULL);
}

Node* genProcedure(char* procName, Node* params, Node* statements)
{
	return makeProcedure(procName, "", "", params, statements);
}

void writeC(t_compiler* compiler, Node* node)
{
	// TODO optimizer kicks in here, AST ready, optimize away!
	makeCode(compiler->fout_name, node);
	dealloc(node);
}

