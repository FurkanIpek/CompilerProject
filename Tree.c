#include "Tree.h"
#include "Logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int logger_id = 2;

void copyStrings(Node* node, char* var, char* label, char* code)
{
	debugInfo("Copying strings to node", logger_id);

	node->var = strdup(var);
	node->label = strdup(label);
	node->code = strdup(code);
}

void generateIfElse(Node* node, FILE* file)
{
	// if node->left->code(condition) GOTO label
	fprintf(file, "if %s GOTO %s\n", node->children[0]->code, node->label); // TODO label?
	// if body
	fprintf(file, "%s\n", node->children[1]->code);
	// else body
	fprintf(file, "%s: %s\n", node->label, node->children[2]->code);
}

void generateWhileLoop(Node* node, FILE* file)
{
	// initLabel: code \n conditional jump
	fprintf(file, "%s: %s\n", node->label, node->children[0]->code);
	fprintf(file, "%s\n", node->children[1]->code);
	fprintf(file, "if %s GOTO %s\n", node->children[0]->code, node->label); // TODO label
}

void generateProcedure(Node* node, FILE* file)
{
	// TODO
	debugInfo("Generating procedure..", logger_id);
}

void generateCode(Node* node, FILE* file)
{
	switch (node->type)
	{
		case leaf: fprintf(file, "%s", node->var); break;

		case operator: fprintf(file, "%s", node->code); break;

		case uOperator: fprintf(file, "%s", node->code); break;

		case ifElse: generateIfElse(node, file); break;

		case whileLoop: generateWhileLoop(node, file); break;

		case procedure: generateProcedure(node, file); break;

		case root: fprintf(file, "%s", node->code); break;
	}
}

void makeCode(Node* node, char* fileName)
{
	FILE* file = fopen(fileName, "w");
	
	if (file != NULL) 
	{
		debugInfo("Generating intermediate code..", logger_id);
		generateCode(node, file);
	}
}

Node* makeLeaf(char* var, char* label, char* code)
{
	debugInfo("Making leaf..", logger_id);

	Node* node = (Node*) malloc (sizeof(Node));
	copyStrings(node, var, label, code);
	node->children[0] = NULL, node->children[1] = NULL, node->children[2] = NULL;
	node->type = leaf;

	return node;
}

Node* makeNode(char* var, char* label, char* code, Node* left, Node* right)
{
	debugInfo("Making node..", logger_id);

	Node* node = (Node*) malloc (sizeof(Node));
	copyStrings(node, var, label, code);
	node->children[0] = left, node->children[1] = NULL, node->children[2] = right;
	node->type = operator;

	return node;
}

Node* makeUNode(char* var, char* label, char* code, Node* left)
{
	debugInfo("Making unary node..", logger_id);

	Node* node = (Node*) malloc (sizeof(Node));
	copyStrings(node, var, label, code);
	node->children[0] = left, node->children[1] = NULL, node->children[2] = NULL;
	node->type = uOperator;

	return node;
}

Node* makeIfElseNode(char* var, char* label, char* code, Node* left, Node* middle, Node* right)
{
	debugInfo("Making if else node..", logger_id);

	Node* node = (Node*) malloc (sizeof(Node));
	copyStrings(node, var, label, code);
	// left -> condition, middle -> if body, right -> else body
	node->children[0] = left, node->children[1] = middle, node->children[2] = right;
	node->type = ifElse;

	return node;
}

Node* makeWhileNode(char* var, char* label, char* code, Node* left, Node* right)
{
	debugInfo("Making while loop node..", logger_id);

	Node* node = (Node*) malloc (sizeof(Node));
	copyStrings(node, var, label, code);
	// left -> condition, right -> body
	node->children[0] = left, node->children[1] = NULL, node->children[2] = right;
	node->type = whileLoop;

	return node;
}

Node* makeProcedure(char* var, char* label, char* code, Node* left, Node* right)
{
	debugInfo("Making procedure..", logger_id);

	Node* node = (Node*) malloc (sizeof(Node));
	copyStrings(node, var, label, code);
	// left -> TODO params, right -> body
	node->children[0] = left, node->children[1] = NULL, node->children[2] = right;
	node->type = procedure;

	return node;
}

Node* makeRoot(char* var, char* label, char* code, Node* left, Node* middle, Node* right)
{
	debugInfo("Making root..", logger_id);

	Node* node = (Node*) malloc (sizeof(Node));
	copyStrings(node, var, label, code);
	node->children[0] = left, node->children[1] = middle, node->children[2] = right;
	node->type = root;

	return node; 
}

void* dealloc(Node* node)
{
	debugInfo("Freeing..", logger_id);

	if (node != NULL)
	{
		if (node->children[0] != NULL)
			dealloc(node->children[0]);

		if (node->children[1] != NULL)
			dealloc(node->children[1]);

		if (node->children[2] != NULL)
			dealloc(node->children[2]);

		free(node->var); free(node->label); free(node->code);		
		free(node);
	}
}

