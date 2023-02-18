#include <fstream>
#include "mylexer.hpp"

int main(int argc, char *argv[]){
	std::ifstream * infile = new std::ifstream(argv[1]);
	MyLexer * lex  = new MyLexer(infile);
	while (true){
		Token * tok;
		int res = lex->produceToken(&tok);
		if (res == ERROR){
			std::cerr << "BAD CHARACTER\n";
			return 1;
		}
		if (res == NUM) {
			std::cout << "found a number\n";
			std::cout << "number was " << tok->toString() << "\n";
		}
		if (res == PLUS){
			std::cout << "found a plus\n";
		}

		if (res == END_OF_FILE){
			break;
		}
	}
}
