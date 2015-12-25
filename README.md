Please see parser.y for the input grammar.

Output grammar for TAC(three address code) is as follows:


Program	-> StmtLst
StmtLst	-> LStmt StmtLst | LStmt
LStmt	-> Lbl: LStmt | Stmt
Stmt	-> AsgnStmt | CondJump | Jump | Param | Call | Return
AsgnStmt-> id = Expr
IfStmt	-> "if" ( id ) "GOTO" Lbl
Jump	-> "GOTO" Lbl
Param	-> "param" id
Call	-> "call" id
Return	-> "return"
Lbl		-> id
Expr	-> Operand | Operand Op Operand
Operand	-> integer | true | false | id
Op		-> + | * | < | == | and


