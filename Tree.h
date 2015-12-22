#pragma once

typedef enum Type {leaf, operator, uOperator, ifElse, whileLoop, procedure, root} Type;

typedef struct Node {
	struct Node* children [3];
	char* var, * label, * code;
	Type type;
} Node;

void makeCode(Node* node, char* fileName);

Node* makeLeaf(char* var, char* label, char* code);
Node* makeNode(char* var, char* label, char* code, Node* left, Node* right);
Node* makeUNode(char* var, char* label, char* code, Node* left);
Node* makeIfElseNode(char* var, char* label, char* code, Node* left, Node* middle, Node* right);
Node* makeWhileNode(char* var, char* label, char* code, Node* left, Node* right);
Node* makeProcedure(char* var, char* label, char* code, Node* left, Node* right);

Node* makeRoot(char* var, char* label, char* code, Node* left, Node* middle, Node* right);

void* dealloc(Node* node);

