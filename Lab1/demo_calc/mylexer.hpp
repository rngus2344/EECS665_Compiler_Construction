#ifndef EECS665_cool_lexer
#define EECS665_cool_lexer

#include <istream>

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif

const int END_OF_FILE = 0;
const int NUM = 1;
const int PLUS = 2;
const int TIMES = 3;
const int MINUS = 4;
const int DIVIDE = 5;
const int ERROR = -1;

class Token{
public:
	virtual std::string toString(){ return "TOKEN"; }
};

class NumToken : public Token{
public:
	NumToken(std::string input) : myVal(input){ }
	virtual std::string toString(){ return myVal; }
	const std::string myVal;
};

class MyLexer : public yyFlexLexer{
public:
	MyLexer(std::istream * in) : yyFlexLexer(in){ }
	int produceToken(Token ** tok);
};

#endif
