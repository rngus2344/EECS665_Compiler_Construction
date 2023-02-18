#ifndef CMINUSMINUS_NAME_ERROR_REPORTING_HH
#define CMINUSMINUS_NAME_ERROR_REPORTING_HH

#include "errors.hpp"

namespace cminusminus{

class NameErr{
public:
static bool undeclID(Position * pos){
	Report::fatal(pos, "Undeclared identifier");
	return false;
}
static bool badVarType(Position * pos){
	Report::fatal(pos, "Invalid type in declaration");
	return false;
}
static bool multiDecl(Position * pos){
	Report::fatal(pos, "Multiply declared identifier");
	return false;
}
};

} //End namespace cminusminus

#endif
