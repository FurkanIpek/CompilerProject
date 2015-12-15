%{
#include "CodeGen.h"
#include "Parser.h"
#include "Lexer.h"

int yywrap() { return 0; }

extern t_compiler* compiler;
char line [256]; // generated lines will be copied into this array
char label [10]; // new labels will be copied into this array
char var [10]; // new temp var.s will be copied into this array
%}

%output  "Parser.c"
%defines "Parser.h"

%union {
	struct {
		char* var;
		char* code;	
	}type;
	char* code;
}

%token <type> tIDENT
%token <type> tINT
%token tTRUE tFALSE tIF tTHEN tELSE tWHILE tBEGIN tEND tENDPROC tAND
tPLUS tMULT tPROC tMAIN tENDMAIN tSEMI tASSIGN tNEWLINE tSMALLER tISEQUAL

%left tAND tISEQUAL tSMALLER tPLUS tMULT

%%
Program: Procedures Main { YYACCEPT; };

Procedures
	: Procedures Proc {}
	| {}
	;

Proc
	: tPROC tIDENT '(' Params ')' StmtLst tENDPROC {}
	;

Params
	: Params ',' tIDENT {}
	| tIDENT {} 
	| {}
	;

Main
	: tMAIN StmtLst tENDMAIN {}
	;

StmtBlk
	: tBEGIN StmtLst tEND {}
	;

StmtLst
	: Stmt StmtLst {}
	| Stmt {}
	;

Stmt
	: AsgnStmt {}
	| IfStmt {}
	| WhlStmt {}
	| CallStmt {}
	;

AsgnStmt
	: tIDENT tASSIGN Expr tSEMI 
{ 
	snprintf(line, 256, "%s%s%s", $<type>1.var, " = ", $<type>3.var);
	writeC(compiler, line, 1);
}
	;

IfStmt
	: tIF '(' Expr ')' tTHEN StmtBlk tELSE StmtBlk {}
	;

WhlStmt
	: tWHILE '(' Expr ')' StmtBlk {}
	;

CallStmt
	: tIDENT '(' ExprList ')' tSEMI
{
	snprintf(line, 256, "%s%s", "call ", $<type>1.var);
	writeC(compiler, line, 1);
}
	;

ExprList /* TODO is removal of ExprList2 correct */
	: ExprList ',' Expr {}
	| Expr { writeC(compiler, $<type>1.code, 1); }
	| {}
	;

Expr
	: tINT { $<type>$.var = $<type>1.var; $<type>$.code = ""; }
	| tFALSE { $<type>$.var = "true"; $<type>$.code = ""; }
	| tTRUE { $<type>$.var = "false"; $<type>$.code = ""; }
	| tIDENT { $<type>$.var = $<type>1.var; $<type>$.code = ""; }
	| '(' Expr ')' { $<type>$.var = $<type>2.var; $<type>$.code = $<type>2.code; }
	| Expr Op Expr
{
	char codex [256];
	$<type>$.var = (char*) malloc(sizeof(char) * 10);
	$<type>$.code = (char*) malloc(sizeof(char) * 256);

	genTemp(var);
	strcpy($<type>$.var, var);
	snprintf(line, 256, "%s%s%s%s%s", $<type>$.var, " = ", $<type>1.var, $<code>2, $<type>3.var);
	writeC(compiler, line, 1);
	snprintf(codex, 256, "%s%s%s", $<type>1.code, $<type>3.code, line);
	strcpy($<type>$.code, codex);
}
	;

Op
	: tPLUS { $<code>$ = " + "; }
	| tMULT { $<code>$ = " * "; }
	| tSMALLER { $<code>$ = " < "; }
	| tISEQUAL { $<code>$ = " == "; }
	| tAND { $<code>$ = " and "; }
	;
%%

