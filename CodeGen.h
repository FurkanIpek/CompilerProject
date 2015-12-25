#ifndef UTILS_H
#define UTILS_H

#include "Parser.h"
#include "Lexer.h"
#include "Tree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct t_compiler {
	char* fin_name, * fout_name;
	FILE* fin, * fout;
} t_compiler;

/* UTILITIES */
void genTemp(char* var);
void genLabel(char* label);
int numChildren(Node* node);

/* LIBRARY */
t_compiler* initializeCodeGenerator(char* fin, char* fout);
void freeCodeGenerator(t_compiler* compiler);

Node* genLeaf(char* var, char* label, char* code);
Node* expression(char* operator, Node* opr1, Node* opr2);
Node* callStatement(char* procName, Node* exprList);
Node* newBranch(Type type, int size);
void extendBranch(Node* branch, Node* statement, Node* bag);
Node* whileStatement(Node* expr, Node* statements);
Node* ifStatement(Node* expr, Node* ifBody, Node* elseBody);
Node* assignStatement(char* var, Node* expr);
Node* genProcedure(char* procName, Node* params, Node* statements);
Node* genRoot(Node* procList, Node* main);

void loadFile(t_compiler* compiler);
int parse(t_compiler* compiler);
void writeC(t_compiler* compiler, Node* node);

#endif
