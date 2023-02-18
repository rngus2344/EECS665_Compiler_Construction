#include <assert.h>

#include "name_analysis.hpp"
#include "type_analysis.hpp"

namespace cminusminus {

TypeAnalysis * TypeAnalysis::build(NameAnalysis * nameAnalysis){
	TypeAnalysis * typeAnalysis = new TypeAnalysis();
	auto ast = nameAnalysis->ast;	
	typeAnalysis->ast = ast;

	ast->typeAnalysis(typeAnalysis);
	if (typeAnalysis->hasError){
		return nullptr;
	}

	return typeAnalysis;

}

void ProgramNode::typeAnalysis(TypeAnalysis * typing){
	for (auto decl : *myGlobals){
		decl->typeAnalysis(typing);
	}
	typing->nodeType(this, BasicType::VOID());
	typing->nodeIsLVal(this, false);
}

void IDNode::typeAnalysis(TypeAnalysis * typing){
	assert(mySymbol != nullptr);
	const DataType * type = mySymbol->getDataType();
	typing->nodeType(this, type);
	typing->nodeIsLVal(this, true);
}

void VarDeclNode::typeAnalysis(TypeAnalysis * typing){
	myType->typeAnalysis(typing);
	const DataType * declaredType = typing->nodeType(myType);
	//We assume that the type that comes back is valid,
	// otherwise we wouldn't have passed nameAnalysis
	typing->nodeType(this, declaredType);
	typing->nodeIsLVal(this, false);
}

void FnDeclNode::typeAnalysis(TypeAnalysis * typing){
	myRetType->typeAnalysis(typing);
	const DataType * retDataType = typing->nodeType(myRetType);

	auto formalTypes = new std::list<const DataType *>();
	for (auto formal : *myFormals){
		formal->typeAnalysis(typing);
		formalTypes->push_back(typing->nodeType(formal));
	}	

	typing->nodeType(this, new FnType(formalTypes, retDataType));
	typing->nodeIsLVal(this, false);

	typing->setCurrentFnType(typing->nodeType(this)->asFn());
	for (auto stmt : *myBody){
		stmt->typeAnalysis(typing);
	}
	typing->setCurrentFnType(nullptr);
}

static bool validAssignOpd(const DataType * type){
	if (type->isBool() || type->isInt() || type->isPtr() || type->isShort()){ 
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

void AssignExpNode::typeAnalysis(TypeAnalysis * typing){
	myDst->typeAnalysis(typing);
	mySrc->typeAnalysis(typing);
	const DataType * dstType = typing->nodeType(myDst);
	const DataType * srcType = typing->nodeType(mySrc);

	/*
	if (dstType->asFn() && srcType->asFn()){
		typing->nodeType(this, ErrorType::produce());
		typing->errAssignFn(this->pos());
		return;
	}
	*/
	if (srcType->isShort() && dstType->isInt()){
		mySrc = new ShortToIntNode(mySrc->pos(), mySrc);
		typing->nodeType(mySrc, BasicType::INT());
		typing->nodeType(this, BasicType::INT());

		return;
	}


	bool validOperands = true;
	bool knownError = type_isError(dstType) || type_isError(srcType);
	if (!validAssignOpd(dstType)){
		typing->errAssignOpd(myDst->pos());
		validOperands = false;
	}
	if (!validAssignOpd(srcType)){
		typing->errAssignOpd(mySrc->pos());
		validOperands = false;
	}
	if (!validOperands || knownError){
		typing->nodeType(this, ErrorType::produce());
		return;
	}

	if (!typing->nodeIsLVal(myDst)){
		typing->nodeType(this, ErrorType::produce());
		return;
	}
	typing->nodeIsLVal(this, false);

	if (dstType == srcType){
		typing->nodeType(this, dstType);
		return;
	}

	typing->nodeType(this, ErrorType::produce());
	typing->errAssignOpr(this->pos());
	return;
}

void CallExpNode::typeAnalysis(TypeAnalysis * typing){
	typing->nodeIsLVal(this, false);

	std::list<const DataType *> * aList = new std::list<const DataType *>();
	for (auto actual : *myArgs){
		actual->typeAnalysis(typing);
		aList->push_back(typing->nodeType(actual));
	}

	SemSymbol * calleeSym = myID->getSymbol();
	assert(calleeSym != nullptr);
	const DataType * calleeType = calleeSym->getDataType();
	const FnType * fnType = calleeType->asFn();
	if (fnType == nullptr){
		typing->errCallee(myID->pos());
		typing->nodeType(this, ErrorType::produce());
		return;
	}

	const std::list<const DataType *>* fList = fnType->getFormalTypes();
	if (aList->size() != fList->size()){
		typing->errArgCount(pos());
		//Note: we still consider the call to return the 
		// return type
	} else {
		auto actualTypesItr = aList->begin();
		auto formalTypesItr = fList->begin();
		auto actualsItr = myArgs->begin();
		while(actualTypesItr != aList->end()){
			const DataType * actualType = *actualTypesItr;
			const DataType * formalType = *formalTypesItr;
			ExpNode * actual = *actualsItr;
			auto actualsItrOld = actualsItr;
			actualTypesItr++;
			formalTypesItr++;
			actualsItr++;

			//Matching to error is ignored
			if (actualType->asError()){ continue; }
			if (formalType->asError()){ continue; }

			//Ok match
			if (formalType == actualType){ continue; }

			//Promote
			if (formalType->isInt() && actualType->isShort()){
				//Promote
				ShortToIntNode * up;
				up = new ShortToIntNode(actual->pos(), actual);
				typing->nodeType(up, BasicType::INT());
				
				actualsItrOld = myArgs->erase(actualsItrOld);
				actualsItrOld = myArgs->insert(actualsItrOld, up);
				
				continue;
			}

			//Bad match
			typing->errArgMatch(actual->pos());
			typing->nodeType(this, ErrorType::produce());
		}
	}

	typing->nodeType(this, fnType->getReturnType());
	return;
}

void NegNode::typeAnalysis(TypeAnalysis * typing){
	typing->nodeIsLVal(this, false);

	myExp->typeAnalysis(typing);
	const DataType * subType = typing->nodeType(myExp);
	typing->nodeIsLVal(this, false);

	//Propagate error, don't re-report
	if (subType->asError()){
		typing->nodeType(this, subType);
		return;
	} else if (subType->isInt()){
		typing->nodeType(this, BasicType::INT());
	} else if (subType->isShort()){
		typing->nodeType(this, BasicType::SHORT());
	} else {
		typing->errMathOpd(myExp->pos());
		typing->nodeType(this, ErrorType::produce());
	}
}

void RefNode::typeAnalysis(TypeAnalysis * typing){
	myID->typeAnalysis(typing);
	typing->nodeIsLVal(this, false);
	if (!typing->nodeIsLVal(myID)){
		typing->errNotLVal(this->pos());
	}
	const DataType * b = typing->nodeType(myID);
	typing->nodeType(this, PtrType::produce(b));
	
}

void DerefNode::typeAnalysis(TypeAnalysis * typing){
	typing->nodeIsLVal(this, true);
	myID->typeAnalysis(typing);
	const DataType * childType = typing->nodeType(myID);
	const PtrType * ptrType = childType->asPtr();
	
	if (ptrType == nullptr){
		typing->errDerefOpd(myID->pos());
		typing->nodeType(this, ErrorType::produce());
		return;
	}

	const DataType * baseType = ptrType->baseType();
	typing->nodeType(this, baseType);
}

void NotNode::typeAnalysis(TypeAnalysis * typing){
	typing->nodeIsLVal(this, false);
	myExp->typeAnalysis(typing);
	const DataType * childType = typing->nodeType(myExp);

	if (childType->asError() != nullptr){
		typing->nodeType(this, ErrorType::produce());
		return;
	}

	if (childType->isBool()){
		typing->nodeType(this, childType);
		return;
	} else {
		typing->errLogicOpd(myExp->pos());
		typing->nodeType(this, ErrorType::produce());
		return;
	}
}

void TypeNode::typeAnalysis(TypeAnalysis * typing){
	typing->nodeType(this, this->getType());
}


static bool typeMathOpd(TypeAnalysis * typing, ExpNode * opd){
	opd->typeAnalysis(typing);
	const DataType * type = typing->nodeType(opd);
	if (type->isInt()){ return true; }
	if (type->isShort()){ return true; }
	if (type->isPtr()){ return true; }
	if (type->asError()){
		//Don't re-report an error, but don't check for
		// incompatibility
		return false;
	}

	typing->errMathOpd(opd->pos());
	return false;
}

/*
static const DataType * getEltType(const ArrayType * arrType){
	if (arrType == nullptr){
		return ErrorType::produce();
	}
	return arrType->baseType();
}
*/

void BinaryExpNode::binaryMathTyping(
	TypeAnalysis * typing
){
	bool lhsValid = typeMathOpd(typing, myExp1);
	bool rhsValid = typeMathOpd(typing, myExp2);
	if (!lhsValid || !rhsValid){
		typing->nodeType(this, ErrorType::produce());
		return;
	}

	//Get the valid operand types, check operator
	const DataType * lhsType = typing->nodeType(myExp1);
	const DataType * rhsType = typing->nodeType(myExp2);

	if (lhsType->isInt() && rhsType->isInt()){
		typing->nodeType(this, BasicType::INT());
		return;
	}
	if (lhsType->isShort() && rhsType->isInt()){
		myExp1 = new ShortToIntNode(myExp1->pos(), myExp1);
		typing->nodeType(myExp1, BasicType::INT());
		typing->nodeType(this, BasicType::INT());
		lhsType = BasicType::INT();
		return;
	}
	if (lhsType->isInt() && rhsType->isShort()){
		myExp2 = new ShortToIntNode(myExp2->pos(), myExp2);
		typing->nodeType(myExp2, BasicType::INT());
		typing->nodeType(this, BasicType::INT());
		rhsType = BasicType::INT();
		return;
	}
	if (lhsType->isShort() && rhsType->isShort()){
		typing->nodeType(this, BasicType::SHORT());
		return;
	}

	if (lhsType->isShort() && rhsType->isInt()){
		myExp1 = new ShortToIntNode(myExp1->pos(), myExp1);
		typing->nodeType(myExp1, BasicType::INT());
		typing->nodeType(this, BasicType::INT());
		lhsType = BasicType::INT();
		return;
	}
	if (lhsType->isInt() && rhsType->isShort()){
		myExp2 = new ShortToIntNode(myExp2->pos(), myExp2);
		typing->nodeType(myExp2, BasicType::INT());
		typing->nodeType(this, BasicType::INT());
		rhsType = BasicType::INT();
		return;
	}
	if (lhsType->isShort() && rhsType->isShort()){
		typing->nodeType(this, BasicType::SHORT());
		return;
	}

	lhsType = typing->nodeType(myExp1);
	rhsType = typing->nodeType(myExp2);
	if (lhsType->isPtr() && rhsType->isPtr()){
		typing->nodeType(this, BasicType::INT());
		return;
	}
	if (lhsType->isPtr() && rhsType->isInt()){
		typing->nodeType(this, lhsType);
		return;
	}
	if (lhsType->isInt() && rhsType->isPtr()){
		typing->nodeType(this, rhsType);
		return;
	}
	

	typing->nodeType(this, ErrorType::produce());
	return;
}

static const DataType * typeLogicOpd(
	TypeAnalysis * typing, ExpNode * opd
){
	opd->typeAnalysis(typing);
	const DataType * type = typing->nodeType(opd);

	//Return type if it's valid
	if (type->isBool()){ return type; }

	//Don't re-report an error, but return null to
	// indicate incompatibility
	if (type->asError()){ return nullptr; }

	//If type isn't an error, but is incompatible,
	// report and indicate incompatibility
	typing->errLogicOpd(opd->pos());
	return NULL;
}

void BinaryExpNode::binaryLogicTyping(TypeAnalysis * typing){
	const DataType * lhsType = typeLogicOpd(typing, myExp1);
	const DataType * rhsType = typeLogicOpd(typing, myExp2);
	if (!lhsType || !rhsType){
		typing->nodeType(this, ErrorType::produce());
		return;
	}

	//Given valid operand types, check operator
	if (lhsType->isBool() && rhsType->isBool()){
		typing->nodeType(this, BasicType::BOOL());
		return;
	}

	//We never expect to get here, so we'll consider it
	// an error with the compiler itself
	throw new InternalError("Incomplete typing");
	typing->nodeType(this, ErrorType::produce());
	return;
}

void PlusNode::typeAnalysis(TypeAnalysis * typing){
	binaryMathTyping(typing);
}

void MinusNode::typeAnalysis(TypeAnalysis * typing){
	binaryMathTyping(typing);
}

void TimesNode::typeAnalysis(TypeAnalysis * typing){
	binaryMathTyping(typing);
}

void DivideNode::typeAnalysis(TypeAnalysis * typing){
	binaryMathTyping(typing);
}

void AndNode::typeAnalysis(TypeAnalysis * typing){
	binaryLogicTyping(typing);
}

void OrNode::typeAnalysis(TypeAnalysis * typing){
	binaryLogicTyping(typing);
}

static const DataType * typeEqOpd(
	TypeAnalysis * typing, ExpNode * opd
){
	assert(opd != nullptr || "opd is null!");

	opd->typeAnalysis(typing);
	const DataType * type = typing->nodeType(opd);

	if (type->isInt()){ return type; }
	if (type->isBool()){ return type; }

	//Errors are invalid, but don't cause re-reports
	if (type->asError()){ return ErrorType::produce(); }

	typing->errEqOpd(opd->pos());
	return ErrorType::produce();
}

void BinaryExpNode::binaryEqTyping(TypeAnalysis * typing){
	const DataType * lhsType = typeEqOpd(typing, myExp1);
	const DataType * rhsType = typeEqOpd(typing, myExp2);

	if (lhsType->asError() || rhsType->asError()){
		typing->nodeType(this, ErrorType::produce());
		return;
	}

	if (lhsType == rhsType){
		typing->nodeType(this, BasicType::BOOL());
		return;
	}

	if (lhsType->isInt() && rhsType->isShort()){
		myExp2 = new ShortToIntNode(myExp2->pos(), myExp2);
		typing->nodeType(myExp2, BasicType::INT());
		typing->nodeType(this, BasicType::BOOL());
		return;
	}
	if (lhsType->isShort() && rhsType->isInt()){
		myExp1 = new ShortToIntNode(myExp1->pos(), myExp1);
		typing->nodeType(myExp1, BasicType::INT());
		typing->nodeType(this, BasicType::BOOL());
		return;
	}

	typing->errEqOpr(pos());
	typing->nodeType(this, ErrorType::produce());
	return;
}

void EqualsNode::typeAnalysis(TypeAnalysis * typing){
	binaryEqTyping(typing);
	assert(typing->nodeType(this) != nullptr);
}

void NotEqualsNode::typeAnalysis(TypeAnalysis * typing){
	binaryEqTyping(typing);
}

static const DataType * typeRelOpd(
	TypeAnalysis * typing, ExpNode * opd
){
	opd->typeAnalysis(typing);
	const DataType * type = typing->nodeType(opd);

	if (type->isInt()){ return type; }
	if (type->isShort()){ return type; }

	//Errors are invalid, but don't cause re-reports
	if (type->asError()){ return nullptr; }

	typing->errRelOpd(opd->pos());
	typing->nodeType(opd, ErrorType::produce());
	return nullptr;
}

void BinaryExpNode::binaryRelTyping(TypeAnalysis * typing){
	const DataType * lhsType = typeRelOpd(typing, myExp1);
	const DataType * rhsType = typeRelOpd(typing, myExp2);

	if (!lhsType || !rhsType){
		typing->nodeType(this, ErrorType::produce());
		return;
	}

	if (lhsType->isInt() && rhsType->isInt()){
		typing->nodeType(this, BasicType::BOOL());
		return;
	}
	if (lhsType->isShort() && rhsType->isInt()){
		myExp1 = new ShortToIntNode(myExp1->pos(), myExp1);
		typing->nodeType(myExp1, BasicType::INT());
		typing->nodeType(this, BasicType::BOOL());
	}
	if (lhsType->isInt() && rhsType->isShort()){
		myExp2 = new ShortToIntNode(myExp2->pos(), myExp2);
		typing->nodeType(myExp2, BasicType::INT());
		typing->nodeType(this, BasicType::BOOL());
	}
	if (lhsType->isShort() && rhsType->isShort()){
		typing->nodeType(this, BasicType::BOOL());
	}

	//There is no bad relational operator, so we never 
	// expect to get here
	return;
}

void GreaterNode::typeAnalysis(TypeAnalysis * typing){
	binaryRelTyping(typing);
}

void GreaterEqNode::typeAnalysis(TypeAnalysis * typing){
	binaryRelTyping(typing);
}

void LessNode::typeAnalysis(TypeAnalysis * typing){
	binaryRelTyping(typing);
}

void LessEqNode::typeAnalysis(TypeAnalysis * typing){
	binaryRelTyping(typing);
}

void AssignStmtNode::typeAnalysis(TypeAnalysis * typing){
	myExp->typeAnalysis(typing);
	const DataType * childType = typing->nodeType(myExp);
	if (childType->asError()){
		typing->nodeType(this, ErrorType::produce());
	} else {
		typing->nodeType(this, BasicType::VOID());
	}
}

void PostDecStmtNode::typeAnalysis(TypeAnalysis * typing){
	myLVal->typeAnalysis(typing);
	const DataType * childType = typing->nodeType(myLVal);

	if (childType->asError()){ return; }
	if (childType->isInt()){ return; }
	if (childType->isShort()){ return; }
	if (childType->asPtr()){ return; }

	//Any other unary math is an error
	typing->errMathOpd(myLVal->pos());
}

void PostIncStmtNode::typeAnalysis(TypeAnalysis * typing){
	myLVal->typeAnalysis(typing);
	const DataType * childType = typing->nodeType(myLVal);

	if (childType->asError()){ return; }
	if (childType->isInt()){ return; }
	if (childType->isShort()){ return; }
	if (childType->asPtr()){ return; }

	//Any other unary math is an error
	typing->errMathOpd(myLVal->pos());
}

void ReadStmtNode::typeAnalysis(TypeAnalysis * typing){
	myDst->typeAnalysis(typing);
	const DataType * childType = typing->nodeType(myDst);

	if (childType->isBool()){
		return;
	} else if (childType->isInt()){
		return;
	} else if (childType->asFn()){
		typing->errReadFn(myDst->pos());
		typing->nodeType(this, ErrorType::produce());
		return;
	} else if (childType->asPtr()){
		typing->errReadPtr(myDst->pos());
		typing->nodeType(this, ErrorType::produce());
		return;
	} else if (childType->asError()){
		typing->nodeType(this, ErrorType::produce());
		return;
	}
	typing->nodeType(this, BasicType::VOID());
}

void WriteStmtNode::typeAnalysis(TypeAnalysis * typing){
	mySrc->typeAnalysis(typing);
	const DataType * childType = typing->nodeType(mySrc);

	//Mark error, but don't re-report
	if (childType->asError()){
		typing->nodeType(this, ErrorType::produce());
		return;
	}

	//Check for invalid type
	if (childType->isVoid()){
		typing->errWriteVoid(mySrc->pos());
		typing->nodeType(this, ErrorType::produce());
		return;
	} else if (childType->asFn()){
		typing->errWriteFn(mySrc->pos());
		typing->nodeType(this, ErrorType::produce());
		return;
	} else if (childType->asBasic()){
		//Can write to a var of any other type
		return;
	}

	if (const PtrType * asPtr = childType->asPtr()){
		const DataType * deref = asPtr->baseType();
		const BasicType * base = deref->asBasic();
		assert(base != nullptr);
			
		typing->errWritePtr(mySrc->pos());
		typing->nodeType(this, BasicType::VOID());
		return;
	}

	typing->nodeType(this, BasicType::VOID());
}

void IfStmtNode::typeAnalysis(TypeAnalysis * typing){
	//Start off the typing as void, but may update to error
	typing->nodeType(this, BasicType::VOID());

	myCond->typeAnalysis(typing);
	const DataType * condType = typing->nodeType(myCond);
	bool goodCond = true;
	if (condType == nullptr){
		typing->nodeType(this, ErrorType::produce());
		goodCond = false;
	} else if (condType->asError()){
		typing->nodeType(this, ErrorType::produce());
		goodCond = false;
	} else if (!condType->isBool()){
		goodCond = false;
		typing->errIfCond(myCond->pos());
		typing->nodeType(this, 
			ErrorType::produce());
	}

	for (auto stmt : *myBody){
		stmt->typeAnalysis(typing);
	}

	if (goodCond){
		typing->nodeType(this, BasicType::produce(VOID));
	} else {
		typing->nodeType(this, ErrorType::produce());
	}
}

void IfElseStmtNode::typeAnalysis(TypeAnalysis * typing){
	myCond->typeAnalysis(typing);
	const DataType * condType = typing->nodeType(myCond);

	bool goodCond = true;
	if (condType->asError()){
		goodCond = false;
		typing->nodeType(this, ErrorType::produce());
	} else if (!condType->isBool()){
		typing->errIfCond(myCond->pos());
		goodCond = false;
	}
	for (auto stmt : *myBodyTrue){
		stmt->typeAnalysis(typing);
	}
	for (auto stmt : *myBodyFalse){
		stmt->typeAnalysis(typing);
	}
	
	if (goodCond){
		typing->nodeType(this, BasicType::produce(VOID));
	} else {
		typing->nodeType(this, ErrorType::produce());
	}
}

void WhileStmtNode::typeAnalysis(TypeAnalysis * typing){
	myCond->typeAnalysis(typing);
	const DataType * condType = typing->nodeType(myCond);

	if (condType->asError()){
		typing->nodeType(this, ErrorType::produce());
	} else if (!condType->isBool()){
		typing->errWhileCond(myCond->pos());
	}

	for (auto stmt : *myBody){
		stmt->typeAnalysis(typing);
	}

	typing->nodeType(this, BasicType::VOID());
}

void CallStmtNode::typeAnalysis(TypeAnalysis * typing){
	myCallExp->typeAnalysis(typing);
	typing->nodeType(this, BasicType::VOID());
}

void ReturnStmtNode::typeAnalysis(TypeAnalysis * typing){
	const FnType * fnType = typing->getCurrentFnType();
	const DataType * fnRet = fnType->getReturnType();

	//Check: shouldn't return anything
	if (fnRet == BasicType::VOID()){
		if (myExp != nullptr) {
			myExp->typeAnalysis(typing);
			typing->extraRetValue(myExp->pos());
			typing->nodeType(this, ErrorType::produce());
		} else {
			typing->nodeType(this, BasicType::VOID());
		}
		return;
	}

	//Check: returns nothing, but should
	if (myExp == nullptr){
		typing->errRetEmpty(pos());
		typing->nodeType(this, ErrorType::produce());
		return;
	}

	myExp->typeAnalysis(typing);
	const DataType * childType = typing->nodeType(myExp);

	if (childType->isShort() && fnRet->isInt()){
		//Promote
		myExp = new ShortToIntNode(myExp->pos(), myExp);
		typing->nodeType(myExp, BasicType::INT());
		return;
	}

	if (childType->asError()){
		typing->nodeType(this, ErrorType::produce());
		return;
	}

	if (childType != fnRet){
		typing->errRetWrong(myExp->pos());
		typing->nodeType(this, ErrorType::produce());
		return;
	}
	typing->nodeType(this, ErrorType::produce());
	return;
}

void StrLitNode::typeAnalysis(TypeAnalysis * typing){
	BasicType * basic = BasicType::STRING();
	//ArrayType * asArr = ArrayType::produce(basic, 0);
	typing->nodeType(this, basic);
}

void FalseNode::typeAnalysis(TypeAnalysis * typing){
	typing->nodeType(this, BasicType::BOOL());
}

void TrueNode::typeAnalysis(TypeAnalysis * typing){
	typing->nodeType(this, BasicType::BOOL());
}

void IntLitNode::typeAnalysis(TypeAnalysis * typing){
	typing->nodeType(this, BasicType::INT());
}

void ShortLitNode::typeAnalysis(TypeAnalysis * typing){
	typing->nodeType(this, BasicType::SHORT());
}

}
