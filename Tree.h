#pragma once

typedef enum Type {t_leaf, t_operator, t_ifElse, t_while, t_exprList, t_stmtList, t_paramList, t_procList, t_procedure, t_procedureCall, t_root, t_none} Type;

typedef struct Node {
	struct Node* children [10]; // only 2 or 3 used in all nodes but branch node
	int num_children; // only used in nodes of type branch
	char* var, * label, * code;
	Type type;
} Node;

void makeCode(char* fileName, Node* node);

Node* makeLeaf(char* var, char* label, char* code);
Node* makeNode(char* var, char* label, char* code, Node* opr1, Node* opr2);
Node* makeIfElseNode(Node* expr, Node* ifBody, Node* elseBody);
Node* makeWhileNode(char* label1, char* label2, Node* expr, Node* statements);
Node* makeBranch(Type type);
void addToBranch(Node* branch, Node* statement, Node* bag);
Node* makeProcedureCall(char* procName, Node* exprList);
Node* makeProcedure(char* var, char* label, char* code, Node* params, Node* statements);
Node* makeRoot(Node* procList, Node* main);

void dealloc(Node* node);

