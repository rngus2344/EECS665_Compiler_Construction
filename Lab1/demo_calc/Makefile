all:
	flex --c++ --yyclass=MyLexer --noyywrap -o lexer.cpp calc.l
	g++ entry.cpp lexer.cpp -o calc
	./calc test.input
