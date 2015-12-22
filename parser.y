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
	char code [256]; // to carry on the code with non-terminals post-fixed with "list"
	void* node; // evaluated as Node* for tree in CodeGen
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
	: tIDENT Params2 {}
	| {}
	;

Params2
	: ',' tIDENT Params2 {}
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
	: tIF '(' Expr ')' tTHEN StmtBlk tELSE StmtBlk
{// if in altına stmtblk1.label sonra code - sonra else için aynısı
// then in sonuna after label!
	snprintf(line, 256, "%s%s%s%s%s%s", "if ", "( ", $<type>3.code, " )", " GOTO ", $<type>3.var);
	writeC(compiler, line, 1);
}
	;

WhlStmt
	: tWHILE '(' Expr ')' StmtBlk
{
	char begin [10];	genLabel(begin);
	char after [10];	genLabel(after);
}
	;

CallStmt
	: tIDENT '(' ExprList ')' tSEMI { makeProcedure(); }
	;

ExprList
	: Expr ExprList2 {}
	| {}
	;

ExprList2
	: ',' Expr ExprList2 { exprList($<code>$, $<node>1); }
	| {}
	;

Expr
	: tINT { $<node>$ = makeLeaf($<type>1.var, "", ""); }
	| tFALSE { $<node>$ = makeLeaf("true", "", ""); }
	| tTRUE { $<node>$ = makeLeaf("false", "", ""); }
	| tIDENT { $<node>$ = makeLeaf($<type>1.var, "", ""); }
	| '(' Expr ')' { $<node>$ = $<node>2; }
	| Expr tPLUS Expr { expression("+", $<node>$, $<node>1, $<node>3); }
	| Expr tMULT Expr { expression("*", $<node>$, $<node>1, $<node>3); }
	| Expr tSMALLER Expr { expression("<", $<node>$, $<node>1, $<node>3); }
	| Expr tISEQUAL Expr { expression("==", $<node>$, $<node>1, $<node>3); }
	| Expr tAND Expr { expression("and", $<node>$, $<node>1, $<node>3); }
	;
%%

