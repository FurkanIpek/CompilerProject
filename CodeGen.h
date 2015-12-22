#ifndef UTILS_H
#define UTILS_H

#include "Parser.h"
#include "Lexer.h"
#include "Tree.h"
#include "Logger.h"

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

/* LIBRARY */
t_compiler* initializeCodeGenerator(char* fin, char* fout);
void freeCodeGenerator(t_compiler* compiler);

void expression(char* operator, Node* base, Node* opr1, Node* opr2);
void callStatement(char* procName);
void exprList(char* str, Node* node);

void loadFile(t_compiler* compiler);
int parse(t_compiler* compiler);
void writeC(t_compiler* compiler, char* str, int newline);

#endif
