%{
#include <string>
#include <limits.h>
#include <iostream>

/* Get our custom yyFlexScanner subclass */
#include "lexer.hpp"
#include "tokens.hpp"
#undef  YY_DECL
#define YY_DECL int cminusminus::Scanner::yylex( cminusminus::Parser::semantic_type * const lval )

/* define yyterminate as this instead of NULL */
#define yyterminate() return( TokenKind::END )

/* Exclude unistd.h for Visual Studio compatability. */
#define YY_NO_UNISTD_H

/* typedef to make the reference to token kinds shorter */
using TokenKind = cminusminus::Parser::token;

%}

%option debug
%option nodefault
%option yyclass="cminusminus::Scanner"
%option noyywrap
%option c++

DIGIT [0-9]
ID [A-Za-z_][A-Za-z0-9_]*
INTLIT 0|[1-9][0-9]*
SHORTLIT 0S|[1-9][0-9]*S
STRLIT ([\\][nt"\\])|([^\\\n"])
BADESCAPE [\\][^nt"\\]?
COMMENT [#][ -~]*

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

int {
			int tokenKind = TokenKind::INT;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+3);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum += 3;
      return tokenKind;
    }
short {
        int tokenKind = TokenKind::SHORT;
        Position * pos = new Position(
          lineNum, colNum, lineNum, colNum+5);
        this->yylval->lexeme = new Token(pos, tokenKind);
        colNum += 5;
        return tokenKind;
      }
bool  {
        int tokenKind = TokenKind::BOOL;
        Position * pos = new Position(
          lineNum, colNum, lineNum, colNum+4);
        this->yylval->lexeme = new Token(pos, tokenKind);
        colNum += 4;
        return tokenKind;
      }
string  {
          int tokenKind = TokenKind::STRING;
          Position * pos = new Position(
            lineNum, colNum, lineNum, colNum+6);
          this->yylval->lexeme = new Token(pos, tokenKind);
          colNum += 6;
          return tokenKind;
        }
void  {
        int tokenKind = TokenKind::VOID;
        Position * pos = new Position(
          lineNum, colNum, lineNum, colNum+4);
        this->yylval->lexeme = new Token(pos, tokenKind);
        colNum += 4;
        return tokenKind;
      }
ptr {
      int tokenKind = TokenKind::PTR;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+3);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum += 3;
      return tokenKind;
    }
gets  {
        int tokenKind = TokenKind::ASSIGN;
        Position * pos = new Position(
          lineNum, colNum, lineNum, colNum+4);
        this->yylval->lexeme = new Token(pos, tokenKind);
        colNum += 4;
        return tokenKind;
      }
if  {
			int tokenKind = TokenKind::IF;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+2);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum += 2;
      return tokenKind;
    }
else  {
        int tokenKind = TokenKind::ELSE;
        Position * pos = new Position(
          lineNum, colNum, lineNum, colNum+4);
        this->yylval->lexeme = new Token(pos, tokenKind);
        colNum += 4;
        return tokenKind;
      }
while {
        int tokenKind = TokenKind::WHILE;
        Position * pos = new Position(
          lineNum, colNum, lineNum, colNum+5);
        this->yylval->lexeme = new Token(pos, tokenKind);
        colNum += 5;
        return tokenKind;
      }
return  {
          int tokenKind = TokenKind::RETURN;
          Position * pos = new Position(
            lineNum, colNum, lineNum, colNum+6);
          this->yylval->lexeme = new Token(pos, tokenKind);
          colNum += 6;
          return tokenKind;
        }
true  {
        int tokenKind = TokenKind::TRUE;
        Position * pos = new Position(
          lineNum, colNum, lineNum, colNum+4);
        this->yylval->lexeme = new Token(pos, tokenKind);
        colNum += 4;
        return tokenKind;
      }
false {
        int tokenKind = TokenKind::FALSE;
        Position * pos = new Position(
          lineNum, colNum, lineNum, colNum+5);
        this->yylval->lexeme = new Token(pos, tokenKind);
        colNum += 5;
        return tokenKind;
      }
read  {
        int tokenKind = TokenKind::READ;
        Position * pos = new Position(
          lineNum, colNum, lineNum, colNum+4);
        this->yylval->lexeme = new Token(pos, tokenKind);
        colNum += 4;
        return tokenKind;
      }
write {
        int tokenKind = TokenKind::WRITE;
        Position * pos = new Position(
          lineNum, colNum, lineNum, colNum+5);
        this->yylval->lexeme = new Token(pos, tokenKind);
        colNum += 5;
        return tokenKind;
      }
or  {
      int tokenKind = TokenKind::OR;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+2);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum += 2;
      return tokenKind;
    }
and {
      int tokenKind = TokenKind::AND;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+3);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum += 3;
      return tokenKind;
    }

"+"	{
			// Keep track of the "kind" of token we matched. In this case,
			// it's a PLUS (aka the plus symbol). The full list of token
			// kinds is declared around line 197 of grammar.hh. The testing
			// functionScanner::outputTokens also indicates what each token
			// represents (declared in scanner.cpp line 9)
			int tokenKind = TokenKind::PLUS;

			Position * pos = new Position(
				lineNum, colNum, lineNum, colNum+1);

			//Instatiate a token. There are 5 classes of tokens defined based on
			// - a literal string (StringLitToken)
			// - a literal integer (IntLitToken)
			// - a literal short (ShortLitToken)
			// - an identifier (IDToken)
			// - a base Token (any other type)
			// The token object is a handy place to keep information about the
			// token (i.e. token metadata) such as the line and column on which
			// the token character started (which can be used later in debug
			// messages, etc).
			Token * token = new Token(pos, tokenKind);

			//Assign the token to a instance variable of the scanner.
			// (When Flex processes a .l file in C++ mode, it creates
			// a class called Scanner, which is a subclass of yyFlexScanner)
			// By assigning a value to this field, we put the token
			// metadata in a place where scanner's client program can find it.
			this->yylval->lexeme = token;

			//Since we consumed 1 character, increase the column number by 1
			colNum++;

			//Let the scanner's client program know that a PLUS token
			// has been matched
			return tokenKind;
		}

"-" {
			//Essentially the same thing as PLUS, but without the verbose
			// comments so it doesn't look so scary.
			int tokenKind = TokenKind::MINUS;
			Position * pos = new Position(
				lineNum, colNum, lineNum, colNum+1);
			this->yylval->lexeme = new Token(pos, tokenKind);
			colNum++;
			return tokenKind;
		}

{SHORTLIT}  {
              int intVal = atoi(yytext);
              double val = std::stod(yytext);
              bool overflow = false;
              bool underflow = false;

              if (val > 32767) { overflow = true; }
              if (val < -32767) { underflow = true; }

              if (overflow) {
                Position * pos = new Position(
                  lineNum, colNum, lineNum, colNum+strlen(yytext));
                std::string msg = "Short literal overflow";
          			Report::fatal(pos,msg);
                colNum += strlen(yytext);
              }

              if (underflow) {
                Position * pos = new Position(
                  lineNum, colNum, lineNum, colNum+strlen(yytext));
                std::string msg = "Short literal underflow";
          			Report::fatal(pos,msg);
                colNum += strlen(yytext);
              }

              int tokenKind = TokenKind::SHORTLITERAL;
  						Position * pos = new Position(
  							lineNum, colNum, lineNum, colNum+strlen(yytext));
  						yylval->lexeme = new ShortLitToken(pos, intVal);
              colNum += strlen(yytext);
  						return tokenKind;
					  }

{DIGIT}+	{
						// TODO: The following computation of the integer
						// value does not check for overflow. It should do so.
						int intVal = atoi(yytext);
            double val = std::stod(yytext);
						bool overflow = false;
            bool underflow = false;

            if (val > INT_MAX) { overflow = true; }
            if (val < INT_MIN) { underflow = true; }

            if (overflow) {
              Position * pos = new Position(
                lineNum, colNum, lineNum, colNum+strlen(yytext));
              std::string msg = "Integer literal overflow";
        			Report::fatal(pos,msg);
              colNum += strlen(yytext);
            }

            if (underflow) {
              Position * pos = new Position(
                lineNum, colNum, lineNum, colNum+strlen(yytext));
              std::string msg = "Integer literal underflow";
        			Report::fatal(pos,msg);
              colNum += strlen(yytext);
            }

						//Note that the IntLitToken does not take a TokenKind argument
						// since it's implicitly known that the token represents an
						// INTLITERAL
						Position * pos = new Position(
							lineNum, colNum, lineNum, colNum+strlen(yytext));
						yylval->lexeme = new IntLitToken(pos, intVal);

						// TODO: update the colNum count according to the number of
						// characters consumed.
            colNum += strlen(yytext);

						//As before, alert the client program that an INTLITERAL
						// token was matched
						return TokenKind::INTLITERAL;
					}

\"{STRLIT}*\"  {
            Position * pos = new Position(
              lineNum, colNum, lineNum, colNum+strlen(yytext));
            yylval->lexeme = new StrToken(pos, yytext);
            colNum += strlen(yytext);
            return TokenKind::STRLITERAL;
          }

\"{STRLIT}*   {
                Position * pos = new Position(
                  lineNum, colNum, lineNum, colNum+strlen(yytext));
                std::string msg = "Unterminated string literal ignored";
                Report::fatal(pos,msg);
                colNum += strlen(yytext);
              }

["]([^"\n]*{BADESCAPE}[^"\n]*)+(\\["])?  {
                                        Position * pos = new Position(
                                          lineNum, colNum, lineNum, colNum+strlen(yytext));
                                        std::string msg = "Unterminated string literal with bad escaped character ignored";
                                        Report::fatal(pos,msg);
                                        colNum += strlen(yytext);
                                      }

["]([^"\n]*{BADESCAPE}[^"\n]*)+["] {
                                  Position * pos = new Position(
                                    lineNum, colNum, lineNum, colNum+strlen(yytext));
                                  std::string msg = "String literal with bad escape seqeunce ignored";
                                  Report::fatal(pos,msg);
                                  colNum += strlen(yytext);
                                }

{ID}  {
        Position * pos = new Position(
          lineNum, colNum, lineNum, colNum+strlen(yytext));
        yylval->lexeme = new IDToken(pos, yytext);
        colNum += strlen(yytext);
        return TokenKind::ID;
      }

" " { colNum++; }

{COMMENT} {
            colNum += strlen(yytext);
          }

"{" {
			int tokenKind = TokenKind::LCURLY;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

"}" {
			int tokenKind = TokenKind::RCURLY;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

"(" {
			int tokenKind = TokenKind::LPAREN;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

")" {
			int tokenKind = TokenKind::RPAREN;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

";" {
			int tokenKind = TokenKind::SEMICOL;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

"@" {
			int tokenKind = TokenKind::SEMICOL;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

"&" {
			int tokenKind = TokenKind::AT;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

"," {
			int tokenKind = TokenKind::COMMA;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

"--" {
			int tokenKind = TokenKind::DEC;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+2);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum += 2;
      return tokenKind;
		}

"*" {
			int tokenKind = TokenKind::TIMES;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

"=" {
			int tokenKind = TokenKind::ASSIGN;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

"!" {
			int tokenKind = TokenKind::NOT;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

"/" {
			int tokenKind = TokenKind::DIVIDE;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

"++" {
			int tokenKind = TokenKind::INC;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+2);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum += 2;
      return tokenKind;
		}

"==" {
			int tokenKind = TokenKind::EQUALS;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+2);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum += 2;
      return tokenKind;
		}

"!=" {
			int tokenKind = TokenKind::NOTEQUALS;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+2);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum += 2;
      return tokenKind;
		}

"<" {
			int tokenKind = TokenKind::LESS;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

">" {
			int tokenKind = TokenKind::GREATER;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+1);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum++;
      return tokenKind;
		}

"<=" {
			int tokenKind = TokenKind::LESSEQ;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+2);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum += 2;
      return tokenKind;
		}

">=" {
			int tokenKind = TokenKind::GREATEREQ;
      Position * pos = new Position(
        lineNum, colNum, lineNum, colNum+2);
      this->yylval->lexeme = new Token(pos, tokenKind);
      colNum += 2;
      return tokenKind;
		}

\n	{
			//Update the line number, reset the columnNumber
			lineNum++;
			colNum = 1;
		}


.   {
			// TODO: Add the rest of the rules above this point so that legal characters
			// are properly accounted for.

			std::string msg = "Illegal character ";
			msg += yytext;
			Position * pos = new Position(
				lineNum, colNum, lineNum, colNum+1);
			Report::fatal(pos,msg);
			colNum += strlen(yytext);
    }
%%
