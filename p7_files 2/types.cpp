#include <list>
#include <sstream>

#include "types.hpp"
#include "ast.hpp"

namespace cminusminus{

std::string BasicType::getString() const{
	std::string res = "";
	switch(myBaseType){
	case BaseType::INT:
		res += "int";
		break;
	case BaseType::BOOL:
		res += "bool";
		break;
	case BaseType::VOID:
		res += "void";
		break;
	case BaseType::SHORT:
		res += "short";
		break;
	case BaseType::STRING:
		res += "string";
		break;
	}
	return res;
}

const DataType * StringTypeNode::getType() { 
	return BasicType::STRING(); 
}

const DataType * BoolTypeNode::getType() { 
	return BasicType::BOOL(); 
}

const DataType * IntTypeNode::getType() { 
	return BasicType::INT(); 
}

const DataType * PtrTypeNode::getType() { 
	return PtrType::produce(myBaseType->getType());
}


} //End namespace
