FILES	= Lexer.c Parser.c CodeGen.c Tree.c main.c
CC	= gcc
CFLAGS	= 

pl4:	$(FILES)
		$(CC) $(CFLAGS) $(FILES) -o pl4

Lexer.c:	lexer.flx 
		flex lexer.flx

Parser.c:	parser.y Lexer.c
		bison parser.y

clean:
		rm -f *.o *~ Lexer.c Lexer.h Parser.c Parser.h pl4
