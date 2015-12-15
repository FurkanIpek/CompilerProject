%{
#include "CodeGen.h"
#include "Parser.h"
#include "Lexer.h"

int yywrap() { return 0; }

extern t_compiler* compiler;
char line [256];
%}

%output  "Parser.c"
%defines "Parser.h"

%union {
	char* str;
}

%token <str> tIDENT
%token <str> tINT
%token tTRUE tFALSE tIF tTHEN tELSE tWHILE tAND tBEGIN tEND tENDPROC
tPROC tMAIN tENDMAIN tPLUS tMULT tSEMI tISEQUAL tASSIGN tSMALLER tNEWLINE

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

Params /* TODO */
	: Params ',' tIDENT {}
	|  tIDENT { writeC(compiler, "MOV tmp, R0\n"); } 
	|  {}
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
	snprintf(line, 256, "%s%s%s", $<str>1, " = ", $<str>3);
	writeC(compiler, line);
}
	;

IfStmt
	: tIF '(' Expr ')' tTHEN StmtBlk tELSE StmtBlk {}
	;

WhlStmt
	: tWHILE '(' Expr ')' StmtBlk {}
	;

CallStmt
	: tIDENT '(' ExprList ')' tSEMI {}
	;

ExprList
	: Expr ExprList2 {}
	|  {}
	;

ExprList2
	: ',' Expr ExprList2 {}
	|  {}
	;

Expr
	: tINT {}
	| tFALSE {}
	| tTRUE {}
	| tIDENT {}
	| '(' Expr ')' {}
	| Expr Op Expr {}
	;

Op
	: tPLUS {}
	| tMULT {}
	| tSMALLER {}
	| tISEQUAL {}
	| tAND {}
	;
%%

