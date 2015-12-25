%{
#include "CodeGen.h"
#include "Parser.h"
#include "Lexer.h"

int yywrap() { return 0; }

extern t_compiler* compiler;
%}

%output  "Parser.c"
%defines "Parser.h"

%union {
	char* var;
	void* node; // evaluated as Node* for tree in CodeGen
}

%token <var> tIDENT
%token <var> tINT
%token tTRUE tFALSE tIF tTHEN tELSE tWHILE tBEGIN tEND tENDPROC tAND
tPLUS tMULT tPROC tMAIN tENDMAIN tSEMI tASSIGN tNEWLINE tSMALLER tISEQUAL

%left tAND tISEQUAL tSMALLER tPLUS tMULT

%%
Program: Procedures Main { $<node>$ = makeRoot($<node>1, $<node>2); writeC(compiler, $<node>$); YYACCEPT; }
	;

Procedures
	: Procedures Proc { $<node>$ = makeBranch(t_procList, getNumChildren($<node>1) + 1); extendBranch($<node>$, $<node>2, $<node>1); }
	| { $<node>$ = makeBranch(t_procList, 0); }
	;

Proc
	: tPROC tIDENT '(' Params ')' StmtLst tENDPROC { $<node>$ = genProcedure($<var>2, $<node>4, $<node>6); }
	;

Params
	: tIDENT Params2 { $<node>$ = makeBranch(t_paramList, getNumChildren($<node>2) + 1); extendBranch($<node>$, genLeaf($<node>1, "", ""), $<node>2); }
	| { $<node>$ = NULL; }
	;

Params2
	: ',' tIDENT Params2 { $<node>$ = makeBranch(t_paramList, getNumChildren($<node>3) + 1); extendBranch($<node>$, genLeaf($<node>2, "", ""), $<node>3); }
	| { $<node>$ = makeBranch(t_paramList, 0); }
	;

Main
	: tMAIN StmtLst tENDMAIN { $<node>$ = $<node>2; }
	;

StmtBlk
	: tBEGIN StmtLst tEND { $<node>$ = $<node>2; }
	;

StmtLst
	: Stmt StmtLst { $<node>$ = makeBranch(t_stmtList, getNumChildren($<node>2) + 1); extendBranch($<node>$, $<node>1, $<node>2); }
	| Stmt { $<node>$ = makeBranch(t_stmtList, 1); extendBranch($<node>$, $<node>1, NULL); }
	;

Stmt
	: AsgnStmt { $<node>$ = $<node>1; }
	| IfStmt { $<node>$ = $<node>1; }
	| WhlStmt { $<node>$ = $<node>1; }
	| CallStmt { $<node>$ = $<node>1; }
	;

AsgnStmt // printf("debug: %i\n", ((Node*)$<node>3)->num_children);
	: tIDENT tASSIGN Expr tSEMI { $<node>$ = assignStatement($<var>1, $<node>3); }
	;

IfStmt
	: tIF '(' Expr ')' tTHEN StmtBlk tELSE StmtBlk { $<node>$ = ifStatement($<node>3, $<node>6, $<node>8); }
	;

WhlStmt
	: tWHILE '(' Expr ')' StmtBlk { $<node>$ = whileStatement($<node>3, $<node>5); }
	;

CallStmt
	: tIDENT '(' ExprList ')' tSEMI { $<node>$ = callStatement($<var>1, $<node>3); }
	;

ExprList
	: Expr ExprList2 { $<node>$ = makeBranch(t_exprList, getNumChildren($<node>2) + 1); extendBranch($<node>$, $<node>1, $<node>2); }
	| { $<node>$ = NULL; }
	;

ExprList2
	: ',' Expr ExprList2 { $<node>$ = makeBranch(t_exprList, getNumChildren($<node>3) + 1); extendBranch($<node>$, $<node>2, $<node>3); }
	| { $<node>$ = makeBranch(t_exprList, 0); }
	;

Expr
	: tINT { $<node>$ = genLeaf($<var>1, "", ""); }
	| tFALSE { $<node>$ = genLeaf("true", "", ""); }
	| tTRUE { $<node>$ = genLeaf("false", "", ""); }
	| tIDENT { $<node>$ = genLeaf($<var>1, "", ""); }
	| '(' Expr ')' { $<node>$ = $<node>2; }
	| Expr tPLUS Expr { $<node>$ = expression("+", $<node>1, $<node>3); }
	| Expr tMULT Expr { $<node>$ = expression("*", $<node>1, $<node>3); }
	| Expr tSMALLER Expr { $<node>$ = expression("<", $<node>1, $<node>3); }
	| Expr tISEQUAL Expr { $<node>$ = expression("==", $<node>1, $<node>3); }
	| Expr tAND Expr { $<node>$ = expression("and", $<node>1, $<node>3); }
	;
%%

