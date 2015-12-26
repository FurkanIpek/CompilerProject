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

int tempVar = 0;
void genTemp(char* var)
{
	char temp [10];
	snprintf(temp, 10, "%s%i", "temp", tempVar++);
	strcpy(var, temp);
}

void checkTempUsage(char* var)
{
	char buff [5]; 
//printf("check: %s\n", var);
	buff[4] = '\0';

	memcpy(buff, &var[0], 4);

	if (strcmp(buff, "temp") == 0 && tempVar > 0) tempVar--;
}

void genLabel(char* label)
{
	static int n = 0;
	char temp [10];
	snprintf(temp, 10, "%s%i", "label", n++);
	strcpy(label, temp);
}

Node* genLeaf(int lineno, char* var, char* label, char* code)
{
	return makeLeaf(lineno, var, label, code);
}

Node* expression(int lineno, char* operator, Node* opr1, Node* opr2)
{
	char temp [10];
	char* code;
	int length = (int)(strlen(operator) + strlen(opr1->code) + strlen(opr2->code)
			+ strlen(temp) + strlen(opr1->var) + strlen(opr2->var)) + 10;
	
	code = (char*) malloc(length * sizeof(char));
	genTemp(temp);
	
	if (strcmp(opr1->code, "") != 0 && opr1->code[strlen(opr1->code)-2] != '\n') strcat(opr1->code, "\n");
	if (strcmp(opr2->code, "") != 0 && opr2->code[strlen(opr2->code)-2] != '\n') strcat(opr2->code, "\n");

	snprintf(code, length, "%s%s%s%s%s%s%s", opr1->code, opr2->code, temp, " = ", opr1->var, operator, opr2->var);

	checkTempUsage(opr1->var);	checkTempUsage(opr2->var);
	
	return makeNode(lineno, temp, operator, code, opr1, opr2);
}

Node* callStatement(int lineno, char* procName, Node* exprList)
{
	return makeProcedureCall(lineno, procName, exprList);
}

int numChildren(Node* node) { return getNumChildren(node); }

Node* newBranch(int lineno, Type type, int size)
{
	return makeBranch(lineno, type, size);
}

void extendBranch(Node* branch, Node* statement, Node* bag)
{
	int i;

	addToBranch(branch, statement, bag);

	for (i = 0; i < branch->num_children; i++)
		if (branch->children[i]->type == t_operator && (branch->type != t_exprList || branch->type != t_paramList))
			checkTempUsage(branch->children[i]->children[0]->var);
}

Node* whileStatement(int lineno, Node* expr, Node* statements)
{
	int i;
	char label1 [10]; genLabel(label1);
	char label2 [10]; genLabel(label2);

	/*for (i = 0; i < statements->num_children; i++)
		checkTempUsage(statements->children[i]->children[0]->var);*/

	return makeWhileNode(lineno, label1, label2, expr, statements);
}

Node* ifStatement(int lineno, Node* expr, Node* ifBody, Node* elseBody)
{
	int i;
	
	/*for (i = 0; i < ifBody->num_children; i++)
		checkTempUsage(ifBody->children[i]->children[0]->var);

	for (i = 0; i < elseBody->num_children; i++)
		checkTempUsage(elseBody->children[i]->children[0]->var);*/

	return makeIfElseNode(lineno, expr, ifBody, elseBody);
}

Node* assignStatement(int lineno, char* var, Node* expr)
{
	char* code;
	int length = (int)(strlen(var) + strlen(expr->code) + strlen(expr->var)) + 5;
	
	code = (char*) malloc(length * sizeof(char));
	
	if (strcmp(expr->code, "") != 0) strcat(expr->code, "\n");

	snprintf(code, length, "%s%s%s%s", expr->code, var, " = ", expr->var);

	return makeNode(lineno, var, "=", code, expr, NULL);
}

Node* genProcedure(int lineno, char* procName, Node* params, Node* statements)
{
	return makeProcedure(lineno, procName, "", "", params, statements);
}

Node* genRoot(int lineno, Node* procList, Node* main)
{
	return makeRoot(lineno, procList, main);
}

int writeC(t_compiler* compiler, Node* node)
{
	if (checkProcedureValidity(node) == 1)
	{
		makeCode(compiler->fout_name, node);
		procCallWarnings(node);
		dealloc(node);

		return 1;
	}
	return 0;
}

