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

void codeLeaf(Node* node)
{
	printf("code leaf\n");
	fprintf(file, "%s", node->var);
}

void codeOperator(Node* node)
{
	printf("code operator\n");
	/*if (node->children[1] == NULL)
	{
		fprintf(file, "%s = ", node->var);
		generateCode(node->children[0]);
		fprintf(file, "\n");
	}
	else
	{
		generateCode(node->children[0]);
		fprintf(file, " %s ", node->label);
		generateCode(node->children[1]);
	}*/
	fprintf(file, "%s", node->code);
}

void codeIfElse(Node* node)
{
	printf("code if else\n");
	fprintf(file, "%s\nif %s", node->children[0]->code, node->children[0]->var);
	fprintf(file, " GOTO %s", node->var);
	generateCode(node->children[2]); // else body
	fprintf(file, "\nGOTO %s\n%s: ", node->label, node->var);
	generateCode(node->children[1]); // if body
	fprintf(file, "\n%s: ", node->label);
}

void codeWhile(Node* node)
{
	printf("code while\n");
	fprintf(file, "%s: ", node->var); //1st label of while statement
	generateCode(node->children[0]); // print exprs
	fprintf(file, "\nif %s == false GOTO %s", node->children[0]->var, node->label);
	generateCode(node->children[1]); // print while body
	fprintf(file, "\nGOTO %s\n", node->var);
	fprintf(file, "%s:", node->label);
}

void codeExprList(Node* node)
{
	int i;
	printf("code expr list\n");
	for (i = 0; i < node->num_children; i++)
		fprintf(file, "param %s\n", node->children[i]->var);
}

void codeStmtList(Node* node)
{
	int i;
	printf("code stmt list\n");
	for (i = 0; i < node->num_children; i++)
		generateCode(node->children[i]);
}

void codeParamList(Node* node)
{
	int i;
	printf("code param list\n");
	for (i = 0; i < node->num_children; i++)
		fprintf(file, " %s", node->children[i]->var);
}

void codeProcList(Node* node)
{
	int i;
	printf("code proc list\n");
	for (i = 0; i < node->num_children; i++)
		generateCode(node->children[i]);
}

void codeProcedure(Node* node)
{
	printf("code procedure\n");
	fprintf(file, "%s:", node->var);
	generateCode(node->children[0]); // TODO should program print formal parameters
	generateCode(node->children[1]); // write function body
	fprintf(file, "return\n");
}

void codeProcedureCall(Node* node)
{
	printf("code procedure call\n");
	generateCode(node->children[0]); // print parameters to be passed on to function
	fprintf(file, "call %s\n", node->var);	
}

void codeRoot(Node* node)
{
	printf("code root\n");
	generateCode(node->children[0]); // proc list
	fprintf(file, "\n-----------------\n\n"); // separate procedures and main to get a good look. coz, u know, looks matter :D
	generateCode(node->children[1]); // main
}

void generateCode(Node* node)
{
	//printf("gen code\n");
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
}

Node* makeLeaf(char* var, char* label, char* code)
{
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, var, label, code);

	node->num_children = 0;
	node->type = t_leaf;

	return node;
}

Node* makeNode(char* var, char* label, char* code, Node* opr1, Node* opr2)
{
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, var, label, code);

	node->children[0] = opr1, node->children[1] = opr2;
	node->num_children = 2;
	node->type = t_operator;

	if (opr2 == NULL) node->num_children = 1; // assignment statements have only 1 child

	return node;
}

Node* makeIfElseNode(Node* expr, Node* ifBody, Node* elseBody)
{
	char l1 [10], l2 [10]; genLabel(l1); genLabel(l2);
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, l1, l2, "");
	
	node->children[0] = expr, node->children[1] = ifBody, node->children[2] = elseBody;
	node->num_children = 3;
	node->type = t_ifElse;

	return node;
}

Node* makeWhileNode(char* label1, char* label2, Node* expr, Node* statements)
{
	Node* node = (Node*) malloc(sizeof(Node));
	// var = label1, label = label2 since we need to labels for each while statement
	copyStrings(node, label1, label2, "");

	node->children[0] = expr, node->children[1] = statements;
	node->num_children = 2;
	node->type = t_while;

	return node;
}

Node* makeBranch(Type type)
{
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, "", "", "");
	node->type = type;
	node->num_children = 0;

	return node;
}

void addToBranch(Node* branch, Node* statement, Node* bag)
{
	int i;
	branch->children[branch->num_children++] = statement;
	
	if (bag != NULL)
		for (i = 0; i < bag->num_children; i++)
			branch->children[branch->num_children++] = bag->children[i];

	free(bag);
}

Node* makeProcedure(char* var, char* label, char* code, Node* params, Node* statements)
{
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, var, label, code);

	node->children[0] = params, node->children[1] = statements;
	node->num_children = 2;
	node->type = t_procedure;

	return node;
}

Node* makeProcedureCall(char* procName, Node* exprList)
{
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, procName, "", "");
	// 1 child which holds expressions that are to be passed to function
	node->children[0] = exprList;
	node->num_children = 1;
	node->type = t_procedureCall;
}

Node* makeRoot(Node* procList, Node* main)
{
	Node* node = (Node*) malloc(sizeof(Node));

	copyStrings(node, "", "", "");

	node->children[0] = procList, node->children[1] = main;
	node->num_children = 2;
	node->type = t_root;
	
	return node; 
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

