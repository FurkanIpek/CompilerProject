#pragma once
/*****************************************************************\

Abstract Syntax Tree(AST) lazy implementation (1 node for all)

date: 25/12/2015
author: Mehmet Furkan İpek

var, label, and code parts in struct "Node" are taken advantage
of in several ways. Their main usage is to hold data about an
"t_operator" node in the following way:

	var = temporary variable assigned to the node
	label = operator, i.e. '+' when a node is created for 5 + 5
	code = i.e. temp = 5 + 5

by using them, outputting TAC(three address code) code becomes easier.

for a t_while node, both var and label are used as label holders since
TAC code for a while loop requires 2 labels, one for jumping to the 
beginning of the loop, one for jumping out of the loop.

for a t_leaf node, var is used to hold value of that node, i.e. false or 5.

see: https://github.com/FurkanIpek/CompilerProject

/*****************************************************************/

typedef enum Type {
	t_leaf,
	t_operator,
	t_ifElse,
	t_while,
	t_exprList,
	t_stmtList,
	t_paramList,
	t_procList,
	t_procedure,
	t_procedureCall,
	t_root,
	t_none
} Type;

typedef struct Node {
	// used as a char pointer array
	struct Node** children;
	// record how many children a node has - helpful for safe dereferencing
	int num_children;
	// explained above
	char* var, * label, * code;
	// see enum definitions
	Type type;
	// line number in the input file
	int line_no;
} Node;

// Generate a TAC file out of AST
void makeCode(char* fileName, Node* node);
// get number of children a node has
int getNumChildren(Node* node);
// free the memory
void dealloc(Node* node);
// check if procedure definitions and procedure calls are valid
int checkProcedureValidity(Node* root); // send the root node
// check whether procedures are used within the code
void procCallWarnings(Node* root);

/**********\\ Generate tree parts //**********/
Node* makeLeaf(int lineno, char* var, char* label, char* code);
Node* makeNode(int lineno, char* var, char* label, char* code, Node* opr1, Node* opr2);
Node* makeIfElseNode(int lineno, Node* expr, Node* ifBody, Node* elseBody);
Node* makeWhileNode(int lineno, char* label1, char* label2, Node* expr, Node* statements);
Node* makeBranch(int lineno, Type type, int size);
void addToBranch(Node* branch, Node* statement, Node* bag);
Node* makeProcedureCall(int lineno, char* procName, Node* exprList);
Node* makeProcedure(int lineno, char* var, char* label, char* code, Node* params, Node* statements);
Node* makeRoot(int lineno, Node* procList, Node* main);
/*********************************************/ 

