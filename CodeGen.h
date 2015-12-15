#ifndef UTILS_H
#define UTILS_H

#include "Parser.h"
#include "Lexer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct t_compiler {
	char* fin_name, * fout_name;
	FILE* fin, * fout;
} t_compiler;

/* UTILITIES */
void usage();
int genTemp();

/* LIBRARY */
t_compiler* initializeCodeGenerator(char* fin, char* fout);
void freeCompiler(t_compiler* compiler);

void loadFile(t_compiler* compiler);
int parse(t_compiler* compiler);
void writeC(t_compiler* compiler, char* str);

#endif
