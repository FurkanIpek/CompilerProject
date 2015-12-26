%{
#include "CodeGen.h"
#include "Parser.h"
#include "Lexer.h"

int yywrap() { return 0; }

int yyerror(const char *msg)
{
    printf("Error in %i: %s\n", yylineno, msg);
	return 0;
}
int getLineNo(Node* node) { return node->line_no; }
extern t_compiler* compiler;
%}

%output  "Parser.c"
%defines "Parser.h"

%union {
	char* var;
	void* node; // evaluated as Node* for tree in "CodeGen"
}

%token <var> tIDENT
%token <var> tINT
%token tTRUE tFALSE tIF tTHEN tELSE tWHILE tBEGIN tEND tENDPROC tAND
tPLUS tMULT tPROC tMAIN tENDMAIN tSEMI tASSIGN tNEWLINE tSMALLER tISEQUAL

%left tAND tISEQUAL tSMALLER tPLUS tMULT

%%
Program: Procedures Main { $<node>$ = genRoot(-1, $<node>1, $<node>2); if (writeC(compiler, $<node>$) == 1) YYACCEPT; else YYABORT; }
	;

Procedures
	: Procedures Proc { $<node>$ = newBranch(-1, t_procList, numChildren($<node>1) + 1); extendBranch($<node>$, $<node>2, $<node>1); }
	| { $<node>$ = newBranch(-1, t_procList, 0); }
	;

Proc
	: tPROC tIDENT '(' Params ')' StmtLst tENDPROC { $<node>$ = genProcedure(getLineNo($<node>4), $<var>2, $<node>4, $<node>6); }
	;

Params
	: tIDENT Params2 { $<node>$ = newBranch(yylineno, t_paramList, numChildren($<node>2) + 1); extendBranch($<node>$, genLeaf(yylineno, $<node>1, "", ""), $<node>2); }
	| { $<node>$ = newBranch(yylineno, t_paramList, 0);; }
	;

Params2
	: ',' tIDENT Params2 { $<node>$ = newBranch(yylineno, t_paramList, numChildren($<node>3) + 1); extendBranch($<node>$, genLeaf(yylineno, $<node>2, "", ""), $<node>3); }
	| { $<node>$ = newBranch(yylineno, t_paramList, 0); }
	;

Main
	: tMAIN StmtLst tENDMAIN { $<node>$ = $<node>2; }
	;

StmtBlk
	: tBEGIN StmtLst tEND { $<node>$ = $<node>2; }
	;

StmtLst // printf("Stmt: %s\n", ((Node*)$<node>1)->var); 
	: Stmt StmtLst { $<node>$ = newBranch(getLineNo($<node>1), t_stmtList, numChildren($<node>2) + 1); extendBranch($<node>$, $<node>1, $<node>2); }
	| Stmt { $<node>$ = newBranch(getLineNo($<node>1), t_stmtList, 1); extendBranch($<node>$, $<node>1, NULL); }
	;

Stmt
	: AsgnStmt { $<node>$ = $<node>1; }
	| IfStmt { $<node>$ = $<node>1; }
	| WhlStmt { $<node>$ = $<node>1; }
	| CallStmt { $<node>$ = $<node>1; }
	;

AsgnStmt
	: tIDENT tASSIGN Expr tSEMI { $<node>$ = assignStatement(getLineNo($<node>3), $<var>1, $<node>3); }
	;

IfStmt
	: tIF '(' Expr ')' tTHEN StmtBlk tELSE StmtBlk { $<node>$ = ifStatement(getLineNo($<node>3), $<node>3, $<node>6, $<node>8); }
	;

WhlStmt
	: tWHILE '(' Expr ')' StmtBlk { $<node>$ = whileStatement(getLineNo($<node>3), $<node>3, $<node>5); }
	;

CallStmt
	: tIDENT '(' ExprList ')' tSEMI { $<node>$ = callStatement(getLineNo($<node>3), $<var>1, $<node>3); }
	;

ExprList
	: Expr ExprList2 { $<node>$ = newBranch(getLineNo($<node>1), t_exprList, numChildren($<node>2) + 1); extendBranch($<node>$, $<node>1, $<node>2); }
	| { $<node>$ = newBranch(yylineno, t_exprList, 0);; }
	;

ExprList2
	: ',' Expr ExprList2 { $<node>$ = newBranch(getLineNo($<node>2), t_exprList, numChildren($<node>3) + 1); extendBranch($<node>$, $<node>2, $<node>3); }
	| { $<node>$ = newBranch(-1, t_exprList, 0); }
	;

Expr
	: tINT { $<node>$ = genLeaf(yylineno, $<var>1, "", ""); }
	| tFALSE { $<node>$ = genLeaf(yylineno, "false", "", ""); }
	| tTRUE { $<node>$ = genLeaf(yylineno, "true", "", ""); }
	| tIDENT { $<node>$ = genLeaf(yylineno, $<var>1, "", ""); }
	| '(' Expr ')' { $<node>$ = $<node>2; }
	| Expr tPLUS Expr { $<node>$ = expression(yylineno, " + ", $<node>1, $<node>3); }
	| Expr tMULT Expr { $<node>$ = expression(yylineno, " * ", $<node>1, $<node>3); }
	| Expr tSMALLER Expr { $<node>$ = expression(yylineno, " < ", $<node>1, $<node>3); }
	| Expr tISEQUAL Expr { $<node>$ = expression(yylineno, " == ", $<node>1, $<node>3); }
	| Expr tAND Expr { $<node>$ = expression(yylineno, " and ", $<node>1, $<node>3); }
	;
%%

