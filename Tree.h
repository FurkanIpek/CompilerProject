#pragma once

typedef enum Type {t_leaf, t_operator, t_ifElse, t_while, t_exprList, t_stmtList, t_paramList, t_procList, t_procedure, t_procedureCall, t_root, t_none} Type;

typedef struct Node {
	struct Node** children;
	int num_children;
	char* var, * label, * code;
	Type type;
} Node;

void makeCode(char* fileName, Node* node);

Node* makeLeaf(char* var, char* label, char* code);
Node* makeNode(char* var, char* label, char* code, Node* opr1, Node* opr2);
Node* makeIfElseNode(Node* expr, Node* ifBody, Node* elseBody);
Node* makeWhileNode(char* label1, char* label2, Node* expr, Node* statements);
Node* makeBranch(Type type, int size);
void addToBranch(Node* branch, Node* statement, Node* bag);
Node* makeProcedureCall(char* procName, Node* exprList);
Node* makeProcedure(char* var, char* label, char* code, Node* params, Node* statements);
Node* makeRoot(Node* procList, Node* main);

int getNumChildren(Node* node);

void dealloc(Node* node);

