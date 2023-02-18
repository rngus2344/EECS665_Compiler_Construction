#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include "types.hpp"
#include "name_analysis.hpp"
#include "type_analysis.hpp"
#include <assert.h>

namespace cminusminus{

TypeAnalysis * TypeAnalysis::build(NameAnalysis * nameAnalysis){
	//To emphasize that type analysis depends on name analysis
	// being complete, a name analysis must be supplied for 
	// type analysis to be performed.
	TypeAnalysis * typeAnalysis = new TypeAnalysis();
	auto ast = nameAnalysis->ast;	
	typeAnalysis->ast = ast;

	ast->typeAnalysis(typeAnalysis);
	if (typeAnalysis->hasError){
		return nullptr;
	}

	return typeAnalysis;

}

void ProgramNode::typeAnalysis(TypeAnalysis * ta){

	//pass the TypeAnalysis down throughout
	// the entire tree, getting the types for
	// each element in turn and adding them
	// to the ta object's hashMap
	for (auto global : *myGlobals){
		global->typeAnalysis(ta);
	}

	//The type of the program node will never
	// be needed. We can just set it to VOID
	//(Alternatively, we could make our type 
	// be error if the DeclListNode is an error)
	ta->nodeType(this, BasicType::produce(VOID));
}

void FnDeclNode::typeAnalysis(TypeAnalysis * ta){

	//HINT: you might want to change the signature for
	// typeAnalysis on FnBodyNode to take a second
	// argument which is the type of the current 
	// function. This will help you to know at a 
	// return statement whether the return type matches
	// the current function

	//Note: this function may need extra code
	myRetType->typeAnalysis(ta);
	const DataType * retDataType = ta->nodeType(myRetType);

	std::list<const DataType *> * formalTypes = new std::list<const DataType *>();
	for (auto formal : *myFormals){
		formal->typeAnalysis(ta);
		formalTypes->push_back(ta->nodeType(formal));
	}

	ta->nodeType(this, new FnType(formalTypes, retDataType));
	ta->setCurrentFnType(ta->nodeType(this)->asFn());

	for (auto stmt : *myBody){
		stmt->typeAnalysis(ta);
	}
	ta->setCurrentFnType(nullptr);
}

void StmtNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Implement me in the subclass");
}

void AssignStmtNode::typeAnalysis(TypeAnalysis * ta){
	myExp->typeAnalysis(ta);

	//It can be a bit of a pain to write 
	// "const DataType *" everywhere, so here
	// the use of auto is used instead to tell the
	// compiler to figure out what the subType variable
	// should be
	auto subType = ta->nodeType(myExp);

	// As error returns null if subType is NOT an error type
	// otherwise, it returns the subType itself
	if (subType->asError()){
		ta->nodeType(this, subType);
	} else {
		ta->nodeType(this, BasicType::produce(VOID));
	}
}

void ExpNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Override me in the subclass");
}

static bool validAssignOpd(const DataType * type){
	if (type == nullptr){
		return false;
	} 

	if (type->isBool() || type->isInt()){ 
		return true; 
	}
	if (type->asError()){ 
		return true; 
	}
	return false;
}

static bool type_isError(const DataType * type){
	return type != nullptr && type->asError();
}

void AssignExpNode::typeAnalysis(TypeAnalysis * ta){
	//TODO: Note that this function is incomplete. 
	// and needs additional code

	//Do typeAnalysis on the subexpressions
	myDst->typeAnalysis(ta);
	mySrc->typeAnalysis(ta);

	const DataType * tgtType = ta->nodeType(myDst);
	const DataType * srcType = ta->nodeType(mySrc);

	//While incomplete, this gives you one case for 
	// assignment: if the types are exactly the same
	// it is usually ok to do the assignment. One
	// exception is that if both types are function
	// names, it should fail type analysis
	if (tgtType == srcType){
		ta->nodeType(this, tgtType);
		return;
	}
	
	//Some functions are already defined for you to
	// report type errors. Note that these functions
	// also tell the typeAnalysis object that the
	// analysis has failed, meaning that main.cpp
	// will print "Type check failed" at the end
	ta->errAssignOpr(this->pos());


	//Note that reporting an error does not set the
	// type of the current node, so setting the node
	// type must be done
	ta->nodeType(this, ErrorType::produce());

	if (tgtType == nullptr || srcType == nullptr ){
		ta->nodeType(this, ErrorType::produce());
		if (tgtType == nullptr){
			ta->errAssignOpd(myDst->pos());
			if (!validAssignOpd(srcType)){ 
				ta->errAssignOpd(mySrc->pos());
			}
		} else if (srcType == nullptr){
			if (!validAssignOpd(tgtType)){ 
				ta->errAssignOpd(myDst->pos());
			}
			ta->errAssignOpd(mySrc->pos());
		}
		return;
	}

	if (tgtType->asFn() && srcType->asFn()){
		ta->nodeType(this, ErrorType::produce());
		ta->errAssignFn(this->pos());
		return;
	}

	bool validOperands = true;
	bool knownError = type_isError(tgtType) || type_isError(srcType);
	if (!validAssignOpd(tgtType)){
		ta->errAssignOpd(myDst->pos());
		validOperands = false;
	}
	if (!validAssignOpd(srcType)){
		ta->errAssignOpd(mySrc->pos());
		validOperands = false;
	}
	if (!validOperands || knownError){
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (tgtType->asError() || srcType->asError()){
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	ta->nodeType(this, ErrorType::produce());
	return;
}

void DeclNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Override me in the subclass");
}

void VarDeclNode::typeAnalysis(TypeAnalysis * ta){
	// VarDecls always pass type analysis, since they 
	// are never used in an expression. You may choose
	// to type them void (like this), as discussed in class
	ta->nodeType(this, BasicType::produce(VOID));
}

void IDNode::typeAnalysis(TypeAnalysis * ta){
	// IDs never fail type analysis and always
	// yield the type of their symbol (which
	// depends on their definition)
	ta->nodeType(this, this->getSymbol()->getDataType());
}

void IntLitNode::typeAnalysis(TypeAnalysis * ta){
	// IntLits never fail their type analysis and always
	// yield the type INT
	ta->nodeType(this, BasicType::produce(INT));
}

void ShortLitNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::produce(SHORT));
}

void StrLitNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::produce(STRING));
}

void TypeNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, this->getType());
}

void ReadStmtNode::typeAnalysis(TypeAnalysis * ta){
	myDst->typeAnalysis(ta);
	const DataType * readType = ta->nodeType(myDst);

	if (readType->asFn()){
		ta->errReadFn(myDst->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	} else if (readType->asPtr()){
		ta->errReadPtr(myDst->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	} else {
		return;
	}
	ta->nodeType(this, BasicType::VOID());
}

void WriteStmtNode::typeAnalysis(TypeAnalysis * ta){
	mySrc->typeAnalysis(ta);
	const DataType * writeType = ta->nodeType(mySrc);

	if (writeType->isVoid()){
		ta->errWriteVoid(mySrc->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	} else if (writeType->asFn()){
		ta->errWriteFn(mySrc->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	} else if (writeType->isPtr()){
		ta->errWritePtr(mySrc->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	} else if (writeType->asBasic()){
		return;
	}

	ta->nodeType(this, BasicType::VOID());
}

void PostDecStmtNode::typeAnalysis(TypeAnalysis * ta){
	myLVal->typeAnalysis(ta);
	const DataType * type = ta->nodeType(myLVal);

	if (type->asError()){ return; }
	if (type->isInt()){ return; }
	ta->errMathOpd(myLVal->pos());
}

void PostIncStmtNode::typeAnalysis(TypeAnalysis * ta){
	myLVal->typeAnalysis(ta);
	const DataType * type = ta->nodeType(myLVal);

	if (type->asError()){ return; }
	if (type->isInt()){ return; }
	ta->errMathOpd(myLVal->pos());
}

void IfStmtNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::VOID());

	myCond->typeAnalysis(ta);
	const DataType * condType = ta->nodeType(myCond);
	bool validCond = true;
	if (condType == nullptr){
		ta->nodeType(this, ErrorType::produce());
		validCond = false;
	} else if (condType->asError()){
		ta->nodeType(this, ErrorType::produce());
		validCond = false;
	} else if (!condType->isBool()){
		validCond = false;
		ta->errIfCond(myCond->pos());
		ta->nodeType(this, 
			ErrorType::produce());
	}

	for (auto stmt : *myBody){
		stmt->typeAnalysis(ta);
	}

	if (validCond){
		ta->nodeType(this, BasicType::produce(VOID));
	} else {
		ta->nodeType(this, ErrorType::produce());
	}
}

void IfElseStmtNode::typeAnalysis(TypeAnalysis * ta){
	myCond->typeAnalysis(ta);
	const DataType * condType = ta->nodeType(myCond);

	bool validCond = true;
	if (condType->asError()){
		validCond = false;
		ta->nodeType(this, ErrorType::produce());
	} else if (!condType->isBool()){
		ta->errIfCond(myCond->pos());
		validCond = false;
	}
	for (auto stmt : *myBodyTrue){
		stmt->typeAnalysis(ta);
	}
	for (auto stmt : *myBodyFalse){
		stmt->typeAnalysis(ta);
	}
	
	if (validCond){
		ta->nodeType(this, BasicType::produce(VOID));
	} else {
		ta->nodeType(this, ErrorType::produce());
	}
}

void WhileStmtNode::typeAnalysis(TypeAnalysis * ta){
	myCond->typeAnalysis(ta);
	const DataType * condType = ta->nodeType(myCond);

	if (condType->asError()){
		ta->nodeType(this, ErrorType::produce());
	} else if (!condType->isBool()){
		ta->errWhileCond(myCond->pos());
	}

	for (auto stmt : *myBody){
		stmt->typeAnalysis(ta);
	}

	ta->nodeType(this, BasicType::VOID());
}

void ReturnStmtNode::typeAnalysis(TypeAnalysis * ta){
	const FnType * fnType = ta->getCurrentFnType();
	const DataType * fnReturn = fnType->getReturnType();

	if (fnReturn == BasicType::VOID()){
		if (myExp != nullptr) {
			myExp->typeAnalysis(ta);
			ta->extraRetValue(myExp->pos());
			ta->nodeType(this, ErrorType::produce());
		} else {
			ta->nodeType(this, BasicType::VOID());
		}
		return;
	}

	if (myExp == nullptr){
		ta->errRetEmpty(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (fnReturn == nullptr){
		ta->errRetWrong(myExp->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	myExp->typeAnalysis(ta);
	const DataType * type = ta->nodeType(myExp);

	if (type->asError()){
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (type != fnReturn){
		ta->errRetWrong(myExp->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, ErrorType::produce());
	return;
}

void CallStmtNode::typeAnalysis(TypeAnalysis * ta){
	myCallExp->typeAnalysis(ta);
	ta->nodeType(this, BasicType::VOID());
}

void CallExpNode::typeAnalysis(TypeAnalysis * ta){

	std::list<const DataType *> * actualList = new std::list<const DataType *>();
	for (auto actual : *myArgs){
		actual->typeAnalysis(ta);
		actualList->push_back(ta->nodeType(actual));
	}

	SemSymbol * calleeSym = myID->getSymbol();
	assert(calleeSym != nullptr);
	const DataType * calleeType = calleeSym->getDataType();
	const FnType * fnType = calleeType->asFn();
	if (fnType == nullptr){
		ta->errCallee(myID->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	const std::list<const DataType *>* fList = fnType->getFormalTypes();
	if (actualList->size() != fList->size()){
		ta->errArgCount(pos());
	} else {
		auto actualTypesItr = actualList->begin();
		auto formalTypesItr = fList->begin();
		auto actualsItr = myArgs->begin();
		while(actualTypesItr != actualList->end()){
			const DataType * actualType = *actualTypesItr;
			const DataType * formalType = *formalTypesItr;
			ExpNode * actual = *actualsItr;
			auto actualsItrOld = actualsItr;
			actualTypesItr++;
			formalTypesItr++;
			actualsItr++;

			if (actualType->asError()){ continue; }
			if (formalType->asError()){ continue; }

			if (formalType == actualType){ continue; }

			ta->errArgMatch(actual->pos());
			ta->nodeType(this, ErrorType::produce());
		}
	}

	ta->nodeType(this, fnType->getReturnType());
	return;
}

static bool typeMathOpd(TypeAnalysis * typing, ExpNode * opd){
	opd->typeAnalysis(typing);
	const DataType * type = typing->nodeType(opd);
	if (type->isInt()){ return true; }
	if (type->asError()){
		return false;
	}

	typing->errMathOpd(opd->pos());
	return false;
}

void BinaryExpNode::binaryMathTyping(TypeAnalysis * ta){
	bool lhsValid = typeMathOpd(ta, myExp1);
	bool rhsValid = typeMathOpd(ta, myExp2);
	if (!lhsValid || !rhsValid){
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	const DataType * lhsType = ta->nodeType(myExp1);
	const DataType * rhsType = ta->nodeType(myExp2);

	if (lhsType->isInt() && rhsType->isInt()){
		ta->nodeType(this, BasicType::INT());
		return;
	}

	ta->nodeType(this, ErrorType::produce());
	return;
}

static const DataType * typeLogicOpd(TypeAnalysis * ta, ExpNode * opd){
	opd->typeAnalysis(ta);
	const DataType * type = ta->nodeType(opd);

	if (type->isBool()){ return type; }

	if (type->asError()){ return nullptr; }

	ta->errLogicOpd(opd->pos());
	return NULL;
}

void BinaryExpNode::binaryLogicTyping(TypeAnalysis * ta){
	const DataType * lhsType = typeLogicOpd(ta, myExp1);
	const DataType * rhsType = typeLogicOpd(ta, myExp2);
	if (!lhsType || !rhsType){
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (lhsType->isBool() && rhsType->isBool()){
		ta->nodeType(this, BasicType::BOOL());
		return;
	}

	throw new InternalError("Incomplete ta");
	ta->nodeType(this, ErrorType::produce());
	return;
}

static const DataType * typeEqOpd(TypeAnalysis * ta, ExpNode * opd){
	assert(opd != nullptr || "opd is null!");

	opd->typeAnalysis(ta);
	const DataType * type = ta->nodeType(opd);
	if (type == nullptr){ return nullptr; } // Record name

	if (type->isInt()){ return type; }
	if (type->isBool()){ return type; }

	if (type->asError()){ return ErrorType::produce(); }

	ta->errEqOpd(opd->pos());
	return ErrorType::produce();
}

void BinaryExpNode::binaryEqTyping(TypeAnalysis * ta){
	const DataType * lhsType = typeEqOpd(ta, myExp1);
	const DataType * rhsType = typeEqOpd(ta, myExp2);

	if (lhsType == nullptr || rhsType == nullptr){
		ta->nodeType(this, ErrorType::produce());
		if (lhsType == nullptr){
			ta->errEqOpd(myExp1->pos());
		}
		return;
	}

	if (lhsType->asError() || rhsType->asError()){
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (lhsType == rhsType){
		ta->nodeType(this, BasicType::BOOL());
		return;
	}

	ta->errEqOpr(pos());
	ta->nodeType(this, ErrorType::produce());
	return;
}

static const DataType * typeRelOpd(TypeAnalysis * ta, ExpNode * opd){
	opd->typeAnalysis(ta);
	const DataType * type = ta->nodeType(opd);

	if (type->isInt()){ return type; }

	if (type->asError()){ return nullptr; }

	ta->errRelOpd(opd->pos());
	ta->nodeType(opd, ErrorType::produce());
	return nullptr;
}

void BinaryExpNode::binaryRelTyping(TypeAnalysis * ta){
	const DataType * lhsType = typeRelOpd(ta, myExp1);
	const DataType * rhsType = typeRelOpd(ta, myExp2);

	if (!lhsType || !rhsType){
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (lhsType->isInt() && rhsType->isInt()){
		ta->nodeType(this, BasicType::BOOL());
		return;
	}
	return;
}

void PlusNode::typeAnalysis(TypeAnalysis * ta){
	binaryMathTyping(ta);
}

void MinusNode::typeAnalysis(TypeAnalysis * ta){
	binaryMathTyping(ta);
}

void TimesNode::typeAnalysis(TypeAnalysis * ta){
	binaryMathTyping(ta);
}

void DivideNode::typeAnalysis(TypeAnalysis * ta){
	binaryMathTyping(ta);
}

void AndNode::typeAnalysis(TypeAnalysis * ta){
	binaryLogicTyping(ta);
}

void OrNode::typeAnalysis(TypeAnalysis * ta){
	binaryLogicTyping(ta);
}

void EqualsNode::typeAnalysis(TypeAnalysis * ta){
	binaryEqTyping(ta);
	assert(ta->nodeType(this) != nullptr);
}

void NotEqualsNode::typeAnalysis(TypeAnalysis * ta){
	binaryEqTyping(ta);
}

void GreaterNode::typeAnalysis(TypeAnalysis * ta){
	binaryRelTyping(ta);
}

void GreaterEqNode::typeAnalysis(TypeAnalysis * ta){
	binaryRelTyping(ta);
}

void LessNode::typeAnalysis(TypeAnalysis * ta){
	binaryRelTyping(ta);
}

void LessEqNode::typeAnalysis(TypeAnalysis * ta){
	binaryRelTyping(ta);
}

void RefNode::typeAnalysis(TypeAnalysis * ta){
	myID->typeAnalysis(ta);
	const DataType * type = ta->nodeType(myID);

	if (type->isPtr()){
		ta->errRefOpd(myID->pos());
		ta->nodeType(this, type);
		return;
	} else {
		ta->errNotLVal(myID->pos());
		ta->nodeType(this, ErrorType::produce());
	}
}

void DerefNode::typeAnalysis(TypeAnalysis * ta){
	myID->typeAnalysis(ta);
	const DataType * type = ta->nodeType(myID);

	if (type->isPtr()){
		ta->nodeType(this, type);
		return;
	} else {
		ta->errDerefOpd(myID->pos());
		ta->nodeType(this, ErrorType::produce());
	}
}

void NegNode::typeAnalysis(TypeAnalysis * ta){
	myExp->typeAnalysis(ta);
	const DataType * subType = ta->nodeType(myExp);

	if (subType->asError()){
		ta->nodeType(this, subType);
		return;
	} else if (subType->isInt()){
		ta->nodeType(this, BasicType::INT());
	} else {
		ta->errMathOpd(myExp->pos());
		ta->nodeType(this, ErrorType::produce());
	}
}

void NotNode::typeAnalysis(TypeAnalysis * ta){
	myExp->typeAnalysis(ta);
	const DataType * type = ta->nodeType(myExp);

	if (type->asError() != nullptr){
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (type->isBool()){
		ta->nodeType(this, type);
		return;
	} else {
		ta->errLogicOpd(myExp->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
}

void TrueNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::BOOL());
}

void FalseNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::BOOL());
}

}
