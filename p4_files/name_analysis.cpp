#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"

namespace cminusminus{

//TODO here is a subset of the nodes needed to do nameAnalysis, 
// you should add the rest to allow for a complete treatment
// of any AST

bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		"been overriden in the subclass!");
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	bool res = true;
	for (auto global : *myGlobals){
		res = global->nameAnalysis(symTab) && res;
	}
	return res;
}

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	std::cerr << "I'm in VarDeclNode throwing an error."
		<< " use GDB or this will look like a segfault!" << std::endl;
	std::cerr << std::flush;
	throw new ToDoError("[DELETE ME] I'm a varDecl"
		" you should add the information from my"	
		" subtree to the symbolTable as a new"	
		" entry in the current scope table"
	);
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	throw new ToDoError("[DELETE ME] I'm an fnDecl."
		" you should add and make current a new"	
		" scope table for my body"
	);
	return nameAnalysisOk;
}

bool IntTypeNode::nameAnalysis(SymbolTable* symTab){
	// Name analysis may never even recurse down to IntTypeNode,
	// but if it does, just return true to indicate that 
	// name analysis has not failed, and add nothing to the symbol table
	return true;
}
}
