#pragma once

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
void tempUsed();
void genLabel(char* label);
int numChildren(Node* node);

/* LIBRARY */
t_compiler* initializeCodeGenerator(char* fin, char* fout);
void freeCodeGenerator(t_compiler* compiler);

Node* genLeaf(int lineno, char* var, char* label, char* code);
Node* expression(int lineno, char* operator, Node* opr1, Node* opr2);
Node* callStatement(int lineno, char* procName, Node* exprList);
Node* newBranch(int lineno, Type type, int size);
void extendBranch(Node* branch, Node* statement, Node* bag);
Node* whileStatement(int lineno, Node* expr, Node* statements);
Node* ifStatement(int lineno, Node* expr, Node* ifBody, Node* elseBody);
Node* assignStatement(int lineno, char* var, Node* expr);
Node* genProcedure(int lineno, char* procName, Node* params, Node* statements);
Node* genRoot(int lineno, Node* procList, Node* main);

void loadFile(t_compiler* compiler);
int parse(t_compiler* compiler);
int writeC(t_compiler* compiler, Node* node);

