#include "ast.hpp"

namespace cminusminus{

IRProgram * ProgramNode::to3AC(TypeAnalysis * ta){
	IRProgram * prog = new IRProgram(ta);
	for (auto global : *myGlobals){
		global->to3AC(prog);
	}
	return prog;
}

void FnDeclNode::to3AC(IRProgram * prog){
	SemSymbol * sSym = this->ID()->getSymbol();
	Procedure * proc = prog->makeProc(sSym->getName());

	for (auto formal : *myFormals){
		formal->to3AC(proc);
	}

	for (auto stmt : *myBody){
		stmt->to3AC(proc);
	}
}

void FnDeclNode::to3AC(Procedure * proc){
	//This never needs to be implemented,
	// the function only exists because of
	// inheritance needs (A function declaration
	// never occurs within another function)
	throw new InternalError("FnDecl at a local scope");
}

void FormalDeclNode::to3AC(IRProgram * prog){
	//This never needs to be implemented,
	// the function only exists because of
	// inheritance needs (A formal never
	// occurs at global scope)
	throw new InternalError("Formal at a global scope");
}

void FormalDeclNode::to3AC(Procedure * proc){
	SemSymbol * sSym = ID()->getSymbol();
	proc->gatherFormal(sSym);
}

Opd * ShortLitNode::flatten(Procedure * proc){
	const DataType * type = proc->getProg()->nodeType(this);
	return new LitOpd(std::to_string(myNum), 1);
}

Opd * IntLitNode::flatten(Procedure * proc){
	const DataType * type = proc->getProg()->nodeType(this);
	return new LitOpd(std::to_string(myNum), 8);
}

Opd * StrLitNode::flatten(Procedure * proc){
	Opd * res = proc->getProg()->makeString(myStr);
	return res;
}

Opd * TrueNode::flatten(Procedure * proc){
	Opd * res = new LitOpd("1", 8);
	return res;
}

Opd * FalseNode::flatten(Procedure * proc){
	Opd * res = new LitOpd("0", 8);
	return res;
}

Opd * AssignExpNode::flatten(Procedure * proc){
	Opd * rhs = mySrc->flatten(proc);
	Opd * lhs = myDst->flatten(proc);
	
	assert(rhs != nullptr);
	assert(lhs != nullptr);

	AssignQuad * quad = new AssignQuad(lhs, rhs);
	quad->setComment("Assign");
	proc->addQuad(quad);
	return lhs;
}

Opd * LValNode::flatten(Procedure * proc){
	TODO(Implement me);
}

Opd * CallExpNode::flatten(Procedure * proc){
	std::list<std::pair<Opd *, const DataType *>> argOpds;

	for (auto argNode : *myArgs){
		Opd * argOpd = argNode->flatten(proc);
		const DataType * argType = proc->getProg()->nodeType(argNode);
		argOpds.push_back(std::make_pair(argOpd, argType));
	}

	size_t argId = 1;

	for (auto argOpd : argOpds){
		Quad * argQuad = new SetArgQuad(argId, argOpd.first);
		proc->addQuad(argQuad);
		argId++;
	}

	Quad * callQuad = new CallQuad(myID->getSymbol());
	proc->addQuad(callQuad);
	SemSymbol * idSym = myID->getSymbol();
	const FnType * calleeType = idSym->getDataType()->asFn();
	const DataType * retType = calleeType->getReturnType();

	if (retType->isVoid()){
		return nullptr;
	}
	else {
		Opd * retVal = proc->makeTmp(Opd::width(retType));
		Quad * getRet = new GetRetQuad(retVal);
		proc->addQuad(getRet);
		return retVal;
	}
}

Opd * NegNode::flatten(Procedure * proc){
	Opd * child = myExp->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * dst = proc->makeTmp(width);
	UnaryOp opr = UnaryOp::NEG64;
	Quad * quad = new UnaryOpQuad(dst, opr, child);
	proc->addQuad(quad);
	return dst;
}

Opd * NotNode::flatten(Procedure * proc){
	Opd * child = myExp->flatten(proc);
	size_t width = proc->getProg()->opWidth(myExp);
	Opd * dst = proc->makeTmp(width);
	UnaryOp opr = UnaryOp::NOT8;
	Quad * quad = new UnaryOpQuad(dst, opr, child);
	proc->addQuad(quad);
	return dst;
}

Opd * PlusNode::flatten(Procedure * proc){
	Opd * childL = myExp1->flatten(proc);
	Opd * childR = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::ADD64;
	Quad * quad = new BinOpQuad(dst, opr, childL, childR);
	proc->addQuad(quad);
	return dst;
}

Opd * MinusNode::flatten(Procedure * proc){
	Opd * childL = myExp1->flatten(proc);
	Opd * childR = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::SUB64;
	Quad * quad = new BinOpQuad(dst, opr, childL, childR);
	proc->addQuad(quad);
	return dst;
}

Opd * TimesNode::flatten(Procedure * proc){
	Opd * childL = myExp1->flatten(proc);
	Opd * childR = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::MULT64;
	Quad * quad = new BinOpQuad(dst, opr, childL, childR);
	proc->addQuad(quad);
	return dst;
}

Opd * DivideNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::DIV64;
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * AndNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * opRes = proc->makeTmp(width);
	BinOpQuad * quad = new BinOpQuad(opRes, AND64, op1, op2);
	proc->addQuad(quad);
	return opRes;
}

Opd * OrNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * opRes = proc->makeTmp(width);
	BinOpQuad * quad = new BinOpQuad(opRes, OR64, op1, op2);
	proc->addQuad(quad);
	return opRes;
}

Opd * EqualsNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	size_t resWidth = Opd::width(BasicType::BOOL());
	Opd * dst = proc->makeTmp(resWidth);
	BinOp opr = BinOp::EQ64;
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * NotEqualsNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	size_t resWidth = Opd::width(BasicType::BOOL());
	Opd * dst = proc->makeTmp(resWidth);
	BinOp opr = BinOp::NEQ64;
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * LessNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	size_t resWidth = Opd::width(BasicType::BOOL());
	Opd * dst = proc->makeTmp(resWidth);
	BinOp opr = BinOp::LT64;
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * GreaterNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	size_t resWidth = Opd::width(BasicType::BOOL());
	Opd * dst = proc->makeTmp(resWidth);
	BinOp opr = BinOp::GT64;
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * LessEqNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	size_t resWidth = Opd::width(BasicType::BOOL());
	Opd * dst = proc->makeTmp(resWidth);
	BinOp opr = BinOp::LTE64;
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * GreaterEqNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	size_t resWidth = Opd::width(BasicType::BOOL());
	Opd * dst = proc->makeTmp(resWidth);
	BinOp opr = BinOp::GTE64;
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * ShortToIntNode::flatten(Procedure * proc){
	Opd* src = myExp -> flatten(proc);
	AuxOpd* auxOpd = proc->makeTmp(8);
	AssignQuad * quad = new AssignQuad(auxOpd, src);
	proc -> addQuad(quad);
	return auxOpd;
}

Opd * RefNode::flatten(Procedure * proc){
	Opd* idOpd = myID -> flatten(proc);
	AuxOpd * auxOpd = proc -> makeTmp(8);
	LocQuad * quad = new LocQuad(idOpd, auxOpd, true, false);
	proc -> addQuad(quad);
	return auxOpd;
}

Opd * DerefNode::flatten(Procedure * proc){
	Opd * idOpd = myID -> flatten(proc);
	AddrOpd * addrOpd = proc -> makeAddrOpd(8);
	LocQuad * quad = new LocQuad(idOpd, addrOpd, false, true);
	proc -> addQuad(quad);
	return addrOpd;
}

void AssignStmtNode::to3AC(Procedure * proc){
	Opd * res = myExp->flatten(proc);
}

void PostIncStmtNode::to3AC(Procedure * proc){
	Opd * child = this->myLVal->flatten(proc);
	size_t width = proc->getProg()->opWidth(myLVal);
	BinOp opr = BinOp::ADD64;
	LitOpd * litOpd = new LitOpd("1", width);
	BinOpQuad * quad = new BinOpQuad(child, opr, child, litOpd);
	proc->addQuad(quad);
}

void PostDecStmtNode::to3AC(Procedure * proc){
	Opd * child = this->myLVal->flatten(proc);
	size_t width = proc->getProg()->opWidth(myLVal);
	BinOp opr = BinOp::SUB64;
	LitOpd * litOpd = new LitOpd("1", width);
	BinOpQuad * quad = new BinOpQuad(child, opr, child, litOpd);
	proc->addQuad(quad);
}

void ReadStmtNode::to3AC(Procedure * proc){
	Opd * child = myDst -> flatten(proc);
	const DataType * type = proc -> getProg() -> nodeType(myDst);
	ReceiveQuad * readQuad = new ReceiveQuad(child, type);
	proc -> addQuad(readQuad);
}

void WriteStmtNode::to3AC(Procedure * proc){
	Opd * child = mySrc -> flatten(proc);
	const DataType * type = proc -> getProg() -> nodeType(mySrc);
	ReportQuad * writeQuad = new ReportQuad(child, type);
	proc -> addQuad(writeQuad);
}

void IfStmtNode::to3AC(Procedure * proc){
	Opd * cond = myCond->flatten(proc);
	Label * afterLabel = proc->makeLabel();
	Quad * afterNop = new NopQuad();
	afterNop->addLabel(afterLabel);

	proc->addQuad(new IfzQuad(cond, afterLabel));
	for (auto stmt : *myBody){
		stmt->to3AC(proc);
	}
	proc->addQuad(afterNop);
}

void IfElseStmtNode::to3AC(Procedure * proc){
	Label * elseLabel = proc->makeLabel();
	Quad * elseNop = new NopQuad();
	elseNop->addLabel(elseLabel);
	Label * afterLabel = proc->makeLabel();
	Quad * afterNop = new NopQuad();
	afterNop->addLabel(afterLabel);

	Opd * cond = myCond->flatten(proc);

	Quad * jmpFalse = new IfzQuad(cond, elseLabel);
	proc->addQuad(jmpFalse);
	for (auto stmt : *myBodyTrue){
		stmt->to3AC(proc);
	}

	Quad * skipFall = new GotoQuad(afterLabel);
	proc->addQuad(skipFall);

	proc->addQuad(elseNop);

	for (auto stmt : *myBodyFalse){
		stmt->to3AC(proc);
	}

	proc->addQuad(afterNop);
}

void WhileStmtNode::to3AC(Procedure * proc){
	Quad * headNop = new NopQuad();
	Label * headLabel = proc->makeLabel();
	headNop->addLabel(headLabel);

	Label * afterLabel = proc->makeLabel();
	Quad * afterQuad = new NopQuad();
	afterQuad->addLabel(afterLabel);

	proc->addQuad(headNop);
	Opd * cond = myCond->flatten(proc);
	Quad * jmpFalse = new IfzQuad(cond, afterLabel);
	proc->addQuad(jmpFalse);

	for (auto stmt : *myBody){
		stmt->to3AC(proc);
	}

	Quad * loopBack = new GotoQuad(headLabel);
	proc->addQuad(loopBack);
	proc->addQuad(afterQuad);
}

void CallStmtNode::to3AC(Procedure * proc){
	Opd * res = myCallExp->flatten(proc);

	if (res != nullptr){
		Quad * last = proc->popQuad();
	}
}

void ReturnStmtNode::to3AC(Procedure * proc){
	if (myExp != nullptr){
		Opd * res = myExp->flatten(proc);
		const DataType * type = proc->getProg()->nodeType(myExp);
		Quad * setOut = new SetRetQuad(res);
		proc->addQuad(setOut);
	}

	Label * leaveLbl = proc->getLeaveLabel();
	Quad * jmpLeave = new GotoQuad(leaveLbl);
	proc->addQuad(jmpLeave);
}

void VarDeclNode::to3AC(Procedure * proc){
	SemSymbol * sSym = ID()->getSymbol();
	assert(sSym != nullptr);
	proc->gatherLocal(sSym);
}

void VarDeclNode::to3AC(IRProgram * prog){
	SemSymbol * sSym = ID()->getSymbol();
	assert(sSym != nullptr);
	prog->gatherGlobal(sSym);
}

//We only get to this node if we are in a stmt
// context (DeclNodes protect descent)
Opd * IDNode::flatten(Procedure * proc){
	SemSymbol * sSym = this->getSymbol();
	Opd * res = proc->getSymOpd(sSym);
	if (!res){
		throw new InternalError("null id sym");;
	}
	return res;
}

}
