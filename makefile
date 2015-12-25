dependencies = Lexer.c Parser.c CodeGen.c CodeGen.h Tree.c Tree.h main.c
files = Lexer.c Parser.c CodeGen.c Tree.c main.c
main_cmd = gcc
flags =
exe_name = -o pl4 

$(exe_name):	$(dependencies)
		$(main_cmd) $(flags) $(files) $(exe_name)

Lexer.c:	lexer.flx 
		flex lexer.flx

Parser.c:	parser.y Lexer.c
		bison parser.y

clean:
		rm -f *.o *~ Lexer.c Lexer.h Parser.c Parser.h pl4
