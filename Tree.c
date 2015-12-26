#include "Tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* file;
void generateCode(Node*);

void copyStrings(Node* node, char* var, char* label, char* code)
{
	node->var = strdup(var);
	node->label = strdup(label);
	node->code = strdup(code);
}

void codeLeaf(Node* node) { fprintf(file, "%s", node->var); }

void codeOperator(Node* node) { fprintf(file, "%s\n", node->code); }

void codeIfElse(Node* node)
{
	fprintf(file, "%s\nif %s", node->children[0]->code, node->children[0]->var);
	fprintf(file, " GOTO %s\n", node->var);
	generateCode(node->children[2]); // else body
	fprintf(file, "GOTO %s\n%s: ", node->label, node->var);
	generateCode(node->children[1]); // if body
	fprintf(file, "%s: ", node->label);
}

void codeWhile(Node* node)
{
	fprintf(file, "%s: ", node->var); //1st label of while statement
	generateCode(node->children[0]); // print exprs
	fprintf(file, "if %s == false GOTO %s\n", node->children[0]->var, node->label);
	generateCode(node->children[1]); // print while body
	fprintf(file, "GOTO %s\n", node->var);
	fprintf(file, "%s: ", node->label);
}

void codeExprList(Node* node)
{
	int i;
	for (i = 0; i < node->num_children; i++)
		fprintf(file, "param %s\n", node->children[i]->var);
}

void codeStmtList(Node* node)
{
	int i;
	for (i = 0; i < node->num_children; i++)
		generateCode(node->children[i]);
}

void codeParamList(Node* node)
{
	int i;
	for (i = 0; i < node->num_children; i++)
		fprintf(file, " %s", node->children[i]->var);
	fprintf(file, "\n");
}

void codeProcList(Node* node)
{
	int i;
	for (i = node->num_children - 1; i >= 0; i--) {
		generateCode(node->children[i]);
		fprintf(file, "\n");
	}
}

void codeProcedure(Node* node)
{
	fprintf(file, "%s %i: ", node->var, node->children[0]->num_children);
	//generateCode(node->children[0]); // formal parameters to the function, hw does not require them to be written
	generateCode(node->children[1]); // write function body
	fprintf(file, "return\n");
}

void codeProcedureCall(Node* node)
{
	generateCode(node->children[0]); // print parameters to be passed on to function
	fprintf(file, "call %s %i\n", node->var, node->children[0]->num_children);	
}

void codeRoot(Node* node)
{
	generateCode(node->children[0]); // proc list
	fprintf(file, "main: ");
	generateCode(node->children[1]); // main
	fprintf(file, "return");
}

void generateCode(Node* node)
{
	switch (node->type)
	{
		case t_leaf: codeLeaf(node); break;

		case t_operator: codeOperator(node); break;

		case t_ifElse: codeIfElse(node); break;

		case t_while: codeWhile(node); break;

		case t_exprList: codeExprList(node); break;

		case t_stmtList: codeStmtList(node); break;

		case t_paramList: codeParamList(node); break;

		case t_procList: codeProcList(node); break;

		case t_procedure: codeProcedure(node); break;

		case t_procedureCall: codeProcedureCall(node); break;

		case t_root: codeRoot(node); break;
	}
}

void makeCode(char* fileName, Node* node)
{
	file = fopen(fileName, "w");
	
	if (file != NULL) 
		generateCode(node);

	fclose(file);
}

Node* makeLeaf(int lineno, char* var, char* label, char* code)
{
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, var, label, code);

	node->line_no = lineno;
	node->children = NULL;
	node->num_children = 0;
	node->type = t_leaf;

	return node;
}

Node* makeNode(int lineno, char* var, char* label, char* code, Node* opr1, Node* opr2)
{
	Node* node = (Node*) malloc(sizeof(Node));
	
	copyStrings(node, var, label, code);
	node->type = t_operator;
	node->line_no = lineno;

	if (opr2 != NULL) {
		node->num_children = 2; // assignment statements have only 1 child
		node->children = (Node**) malloc(node->num_children * sizeof(Node*));
		node->children[0] = opr1, node->children[1] = opr2;
	}
	else {
		node->num_children = 1; // assignment statements have only 1 child
		node->children = (Node**) malloc(sizeof(Node*));
		node->children[0] = opr1;
	}

	return node;
}

Node* makeIfElseNode(int lineno, Node* expr, Node* ifBody, Node* elseBody)
{
	char l1 [10], l2 [10]; genLabel(l1); genLabel(l2);
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, l1, l2, "");

	node->line_no = lineno;	
	node->num_children = 3;
	node->children = (Node**) malloc(node->num_children * sizeof(Node*));
	node->children[0] = expr, node->children[1] = ifBody, node->children[2] = elseBody;
	node->type = t_ifElse;

	return node;
}

Node* makeWhileNode(int lineno, char* label1, char* label2, Node* expr, Node* statements)
{
	Node* node = (Node*) malloc(sizeof(Node));
	// var = label1, label = label2 since we need two labels for each while statement
	copyStrings(node, label1, label2, "");

	node->line_no = lineno;
	node->num_children = 2;
	node->children = (Node**) malloc(node->num_children * sizeof(Node*));
	node->children[0] = expr, node->children[1] = statements;
	node->type = t_while;

	return node;
}

Node* makeBranch(int lineno, Type type, int size)
{
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, "", "", "");

	node->line_no = lineno;
	node->type = type;
	node->num_children = size;
	node->children = (Node**) malloc(node->num_children * sizeof(Node*));

	return node;
}

void addToBranch(Node* branch, Node* statement, Node* bag)
{
	int i;
	branch->children[0] = statement;

	if (bag != NULL) {
		for (i = 0; i < bag->num_children; i++)
			branch->children[i+1] = bag->children[i];

		free(bag);
	}
}

Node* makeProcedure(int lineno, char* var, char* label, char* code, Node* params, Node* statements)
{
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, var, label, code);
	
	node->line_no = lineno;
	node->num_children = 2;
	node->children = (Node**) malloc(node->num_children * sizeof(Node*));
	node->children[0] = params, node->children[1] = statements;
	node->type = t_procedure;
	// TODO
	if (params == NULL)
		node->children[0] = makeBranch(lineno, t_paramList, 0);

	return node;
}

Node* makeProcedureCall(int lineno, char* procName, Node* exprList)
{
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, procName, "", "");
	
	node->line_no = lineno;
	node->num_children = 1;
	node->children = (Node**) malloc(node->num_children * sizeof(Node*));
	node->children[0] = exprList;
	node->type = t_procedureCall;
	// TODO
	if (exprList == NULL)
		node->children[0] = makeBranch(lineno, t_exprList, 0);

	return node;
}

Node* makeRoot(int lineno, Node* procList, Node* main)
{
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, "", "", "");

	node->line_no = lineno;
	node->num_children = 2;
	node->children = (Node**) malloc(node->num_children * sizeof(Node*));
	node->children[0] = procList, node->children[1] = main;
	node->type = t_root;
	
	return node; 
}

int getNumChildren(Node* node) { return node->num_children; }

/* this array will be initiated in checkProcedureValidity function,
 filled in in isProcedureExists function, and used in procCallWarnings
 function as data for producing function not used warnings*/
int* isProcedureCalled;

int isProcedureExists(Node* procList, Node* procCall) 
{
	int i, wrongNumParams = 0;

	for (i = 0; i < procList->num_children; i++)
	{
		if (strcmp(procList->children[i]->var, procCall->var) == 0)
		{
			if (procList->children[i]->children[0]->num_children != procCall->children[0]->num_children) {
				wrongNumParams = 1;
			}
			else { 
				isProcedureCalled[i] = 1;
				return 1;
			}
		}
	}

	if (wrongNumParams == 1)
		printf("Line %i: There is no definition for procedure \"%s\" with %i parameter(s)\n", 
			procCall->line_no ,procCall->var, procCall->children[0]->num_children);
	else
		printf("Line %i: Procedure \"%s\" does not exist.\n", procCall->line_no, procCall->var);
	return 0;
}

// Check statements in the code, if a procedure call is present, check if the procedure exists
int checkProcCallValidity(Node* procList, Node* node)
{
	int i;
	static int valid = 1;
	Node* curNode;
	
	if (node->type == t_stmtList)
	{
		for (i = 0; i < node->num_children; i++)
		{
			curNode = node->children[i];
			switch (curNode->type)
			{
				case t_procedureCall:
					if (isProcedureExists(procList, curNode) != 1)
						valid = 0; // reject
				break;

				case t_ifElse: // if else node has 2 stmtList in it, if body and else body
					checkProcCallValidity(procList, curNode->children[1]);
					checkProcCallValidity(procList, curNode->children[2]);
				break;

				case t_while: // while node has 1 stmtList in it
					checkProcCallValidity(procList, curNode->children[1]);
				break;
			}
		}
	}
	else
	{
		printf("This part is not reachable. If you are seeing this on the console, then it means that some weird shit is goin' on!\n");
		return 0; // reject
	}

	return valid; // accept
}

int checkProcedureValidity(Node* root)
{
	int i, j;
	Node* procList = root->children[0], * main = root->children[1];
	
	isProcedureCalled = (int*) calloc(procList->num_children, sizeof(int));

	// check validity of procedure definitions, if some have same name, their parameter number should differ
	for (i = 0; i < procList->num_children; i++)
	{
		for (j = procList->num_children - 1; j > i; j--)
		{
			if ((strcmp(procList->children[i]->var, procList->children[j]->var) == 0) // procedure names are the same
				&& (procList->children[i]->children[0]->num_children == procList->children[j]->children[0]->num_children)) // parameter numbers are the same
			{
				printf("Error: Overloaded function \"%s\" has at least two instances with the same parameter number.\n", 
						procList->children[i]->var);
				return 0; // reject
			}
		}
	}

	/* now check if procedure calls actually call existing procedures */
	
	// check procedure calls inside the procedure definitions
	for (i = 0; i < procList->num_children; i++)
		// each child of procList is a procedure, each children[1] of a procedure is statement list of that procedure
		if (checkProcCallValidity(procList, procList->children[i]->children[1]) != 1)
			return 0; // reject

	// now check statements in the main
	return checkProcCallValidity(procList, main);
}

void procCallWarnings(Node* root)
{
	int i;
	Node* curNode;

	for (i = 0; i < root->children[0]->num_children; i++)
	{
		curNode = root->children[0]->children[i];
		if (isProcedureCalled[i] == 0)
		{
			printf("Warning: Procedure \"%s(%i params)\" starting at line %i is not used!\n", 
				curNode->var, curNode->children[0]->num_children, curNode->line_no);
		}
	}
}

void dealloc(Node* node)
{
	int i;

	if (node != NULL)
	{
		for (i = 0; i < node->num_children; i++)
			if (node->children[i] != NULL)
				dealloc(node->children[i]);
		
		free(node);
	}
}

