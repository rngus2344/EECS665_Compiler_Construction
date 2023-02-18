#ifndef CMINUSMINUS_AST_HPP
#define CMINUSMINUS_AST_HPP

#include <ostream>
#include <sstream>
#include <string.h>
#include <list>
#include "tokens.hpp"
#include "types.hpp"
#include "3ac.hpp"

namespace cminusminus {

class TypeAnalysis;

class Opd;

class SymbolTable;
class SemSymbol;

class DeclNode;
class VarDeclNode;
class StmtNode;
class AssignExpNode;
class FormalDeclNode;
class TypeNode;
class ExpNode;
class LValNode;
class IDNode;

class ASTNode{
public:
	ASTNode(Position * pos) : myPos(pos){ }
	virtual void unparse(std::ostream&, int) = 0;
	Position * pos() { return myPos; };
	std::string posStr(){ return pos()->span(); }
	virtual bool nameAnalysis(SymbolTable *) = 0;
	//Note that there is no ASTNode::typeAnalysis. To allow
	// for different type signatures, type analysis is 
	// implemented as needed in various subclasses
	virtual std::string nodeKind() = 0;
protected:
	Position * myPos = nullptr;
};

class ProgramNode : public ASTNode{
public:
	ProgramNode(std::list<DeclNode *> * globalsIn);
	virtual std::string nodeKind() override { return "Program"; }
	void unparse(std::ostream&, int) override;
	virtual bool nameAnalysis(SymbolTable *) override;
	virtual void typeAnalysis(TypeAnalysis *);
	IRProgram * to3AC(TypeAnalysis * ta);
	virtual ~ProgramNode(){ }
private:
	std::list<DeclNode *> * myGlobals;
};

class ExpNode : public ASTNode{
protected:
	ExpNode(Position * p) : ASTNode(p){ }
public:
	virtual void unparseNested(std::ostream& out);
	//virtual void unparse(std::ostream& out, int indent) override = 0;
	virtual bool nameAnalysis(SymbolTable * symTab) override = 0;
	virtual void typeAnalysis(TypeAnalysis *) = 0;
	virtual Opd * flatten(Procedure * proc) = 0;
};

class LValNode : public ExpNode{
public:
	LValNode(Position * p) : ExpNode(p){}
	virtual std::string nodeKind() override { return "LVal"; }
	void unparse(std::ostream& out, int indent) override = 0;
	void unparseNested(std::ostream& out) override;
	void attachSymbol(SemSymbol * symbolIn) { } 
	bool nameAnalysis(SymbolTable * symTab) override { return false; }
	virtual void typeAnalysis(TypeAnalysis *) override {; } 
	virtual Opd * flatten(Procedure * proc) override;
};

class IDNode : public LValNode{
public:
	IDNode(Position * p, std::string nameIn)
	: LValNode(p), name(nameIn), mySymbol(nullptr){}
	std::string getName(){ return name; }
	virtual std::string nodeKind() override { return "ID"; }
	void unparse(std::ostream& out, int indent) override;
	void attachSymbol(SemSymbol * symbolIn);
	SemSymbol * getSymbol() const { return mySymbol; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * proc) override;
private:
	std::string name;
	SemSymbol * mySymbol;
};

class TypeNode : public ASTNode{
public:
	TypeNode(Position * p) : ASTNode(p){ }
	void unparse(std::ostream&, int) override = 0;
	virtual std::string nodeKind() override = 0;
	virtual const DataType * getType() = 0;
	virtual bool nameAnalysis(SymbolTable *) override;
	virtual void typeAnalysis(TypeAnalysis *);
};

class StmtNode : public ASTNode{
public:
	StmtNode(Position * p) : ASTNode(p){ }
	virtual void unparse(std::ostream& out, int indent) override = 0;
	virtual std::string nodeKind() override = 0;
	virtual void typeAnalysis(TypeAnalysis *) = 0;
	virtual void to3AC(Procedure * proc) = 0;
};

class DeclNode : public StmtNode{
public:
	DeclNode(Position * p) : StmtNode(p){ }
	void unparse(std::ostream& out, int indent) override =0;
	virtual std::string nodeKind() override = 0;
	virtual void typeAnalysis(TypeAnalysis *) override = 0;
	virtual void to3AC(IRProgram * prog) = 0;
	virtual void to3AC(Procedure * proc) override = 0;
};

class VarDeclNode : public DeclNode{
public:
	VarDeclNode(Position * p, TypeNode * typeIn, IDNode * IDIn)
	: DeclNode(p), myType(typeIn), myID(IDIn){ }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "VarDecl"; }
	IDNode * ID(){ return myID; }
	TypeNode * getTypeNode(){ return myType; }
	bool nameAnalysis(SymbolTable * symTab) override;
	void typeAnalysis(TypeAnalysis * typing) override;
	virtual void to3AC(Procedure * proc) override;
	virtual void to3AC(IRProgram * prog) override;
private:
	TypeNode * myType;
	IDNode * myID;
};

class FormalDeclNode : public VarDeclNode{
public:
	FormalDeclNode(Position * p, TypeNode * type, IDNode * id) 
	: VarDeclNode(p, type, id){ }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "FormalDecl"; }
	virtual void to3AC(Procedure * proc) override;
	virtual void to3AC(IRProgram * prog) override;
};

class FnDeclNode : public DeclNode{
public:
	FnDeclNode(Position * p, 
	  TypeNode * retTypeIn, IDNode * idIn,
	  std::list<FormalDeclNode *> * formalsIn,
	  std::list<StmtNode *> * bodyIn)
	: DeclNode(p), myRetType(retTypeIn), myID(idIn),
	  myFormals(formalsIn), myBody(bodyIn){ 
	}
	IDNode * ID() const { return myID; }
	std::list<FormalDeclNode *> * getFormals() const{
		return myFormals;
	}
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "FnDecl"; }
	virtual bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	void to3AC(IRProgram * prog) override;
	void to3AC(Procedure * prog) override;
	virtual TypeNode * getRetTypeNode() { 
		return myRetType;
	}
private:
	TypeNode * myRetType;
	IDNode * myID;
	std::list<FormalDeclNode *> * myFormals;
	std::list<StmtNode *> * myBody;
};

class AssignStmtNode : public StmtNode{
public:
	AssignStmtNode(Position * p, AssignExpNode * expIn)
	: StmtNode(p), myExp(expIn){ }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "AssignStmt"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual void to3AC(Procedure * prog) override;
private:
	AssignExpNode * myExp;
};

class ReadStmtNode : public StmtNode{
public:
	ReadStmtNode(Position * p, LValNode * dstIn)
	: StmtNode(p), myDst(dstIn){ }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "ReceiveStmt"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual void to3AC(Procedure * prog) override;
private:
	LValNode * myDst;
};

class WriteStmtNode : public StmtNode{
public:
	WriteStmtNode(Position * p, ExpNode * srcIn)
	: StmtNode(p), mySrc(srcIn){ }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "ReportStmt"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual void to3AC(Procedure * prog) override;
private:
	ExpNode * mySrc;
};

class PostDecStmtNode : public StmtNode{
public:
	PostDecStmtNode(Position * p, LValNode * lvalIn)
	: StmtNode(p), myLVal(lvalIn){ }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "PostDecStmt"; }
	virtual bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual void to3AC(Procedure * prog) override;
private:
	LValNode * myLVal;
};

class PostIncStmtNode : public StmtNode{
public:
	PostIncStmtNode(Position * p, LValNode * lvalIn)
	: StmtNode(p), myLVal(lvalIn){ }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "PostIncStmt"; }
	virtual bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual void to3AC(Procedure * prog) override;
private:
	LValNode * myLVal;
};

class IfStmtNode : public StmtNode{
public:
	IfStmtNode(Position * p, ExpNode * condIn,
	  std::list<StmtNode *> * bodyIn)
	: StmtNode(p), myCond(condIn), myBody(bodyIn){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "IfStmt"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual void to3AC(Procedure * prog) override;
private:
	ExpNode * myCond;
	std::list<StmtNode *> * myBody;
};

class IfElseStmtNode : public StmtNode{
public:
	IfElseStmtNode(Position * p, ExpNode * condIn, 
	  std::list<StmtNode *> * bodyTrueIn,
	  std::list<StmtNode *> * bodyFalseIn)
	: StmtNode(p), myCond(condIn),
	  myBodyTrue(bodyTrueIn), myBodyFalse(bodyFalseIn) { }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "IfElseStmt"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual void to3AC(Procedure * prog) override;
private:
	ExpNode * myCond;
	std::list<StmtNode *> * myBodyTrue;
	std::list<StmtNode *> * myBodyFalse;
};

class WhileStmtNode : public StmtNode{
public:
	WhileStmtNode(Position * p, ExpNode * condIn, 
	  std::list<StmtNode *> * bodyIn)
	: StmtNode(p), myCond(condIn), myBody(bodyIn){ }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "WhileStmt"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual void to3AC(Procedure * prog) override;
private:
	ExpNode * myCond;
	std::list<StmtNode *> * myBody;
};

class ReturnStmtNode : public StmtNode{
public:
	ReturnStmtNode(Position * p, ExpNode * exp)
	: StmtNode(p), myExp(exp){ }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "ReturnStmt"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual void to3AC(Procedure * proc) override;
private:
	ExpNode * myExp;
};

class CallExpNode : public ExpNode{
public:
	CallExpNode(Position * p, IDNode * id,
	  std::list<ExpNode *> * argsIn)
	: ExpNode(p), myID(id), myArgs(argsIn){ }
	void unparse(std::ostream& out, int indent) override;
	void unparseNested(std::ostream& out) override;
	virtual std::string nodeKind() override { return "CallExp"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	void typeAnalysis(TypeAnalysis *) override;
	DataType * getRetType();

	virtual Opd * flatten(Procedure * proc) override;
private:
	IDNode * myID;
	std::list<ExpNode *> * myArgs;
};

class BinaryExpNode : public ExpNode{
public:
	BinaryExpNode(Position * p, ExpNode * lhs, ExpNode * rhs)
	: ExpNode(p), myExp1(lhs), myExp2(rhs) { }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override = 0;
	virtual Opd * flatten(Procedure * prog) override = 0;
protected:
	ExpNode * myExp1;
	ExpNode * myExp2;
	void binaryLogicTyping(TypeAnalysis * typing);
	void binaryEqTyping(TypeAnalysis * typing);
	void binaryRelTyping(TypeAnalysis * typing);
	void binaryMathTyping(TypeAnalysis * typing);
};

class PlusNode : public BinaryExpNode{
public:
	PlusNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "Plus"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class MinusNode : public BinaryExpNode{
public:
	MinusNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "Minus"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class TimesNode : public BinaryExpNode{
public:
	TimesNode(Position * p, ExpNode * e1In, ExpNode * e2In)
	: BinaryExpNode(p, e1In, e2In){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "Times"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class DivideNode : public BinaryExpNode{
public:
	DivideNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "Divide"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class AndNode : public BinaryExpNode{
public:
	AndNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "And"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class OrNode : public BinaryExpNode{
public:
	OrNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "Or"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class EqualsNode : public BinaryExpNode{
public:
	EqualsNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "Eq"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
	
};

class NotEqualsNode : public BinaryExpNode{
public:
	NotEqualsNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "NotEq"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
	
};

class LessNode : public BinaryExpNode{
public:
	LessNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "Less"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * proc) override;
};

class LessEqNode : public BinaryExpNode{
public:
	LessEqNode(Position * pos, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(pos, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "LessEq"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class GreaterNode : public BinaryExpNode{
public:
	GreaterNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "GreaterEq"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * proc) override;
};

class GreaterEqNode : public BinaryExpNode{
public:
	GreaterEqNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "GreaterEq"; }
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class UnaryExpNode : public ExpNode {
public:
	UnaryExpNode(Position * p, ExpNode * expIn) 
	: ExpNode(p){
		this->myExp = expIn;
	}
	virtual void unparse(std::ostream& out, int indent) override = 0;
	virtual bool nameAnalysis(SymbolTable * symTab) override = 0;
	virtual void typeAnalysis(TypeAnalysis *) override = 0;
	virtual Opd * flatten(Procedure * prog) override = 0;
protected:
	ExpNode * myExp;
};

class ShortToIntNode : public UnaryExpNode{
public:
	ShortToIntNode(Position * p, ExpNode * expIn): UnaryExpNode(p, expIn) { }
	void unparse(std::ostream& out, int indent) override {
		myExp->unparse(out, indent);
	}
	virtual std::string nodeKind() override { 
		return "ShortToInt"; 
	}
	virtual bool nameAnalysis(SymbolTable * symTab) override { 
		return myExp->nameAnalysis(symTab); 
	}
	virtual void typeAnalysis(TypeAnalysis * ta) override {
		return myExp->typeAnalysis(ta); 
	}
	virtual Opd * flatten(Procedure * prog) override;
};


class RefNode : public UnaryExpNode{
public:
	RefNode(Position * p, IDNode * IDIn) 
	: UnaryExpNode(p, IDIn), myID(IDIn){
	}
	std::string nodeKind() override { return "&"; }
	virtual void unparse(std::ostream& out, int indent) override;
	virtual bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
protected:
	IDNode * myID;
};

class DerefNode : public LValNode{
public:
	DerefNode(Position * p, IDNode * IDIn) 
	: LValNode(p), myID(IDIn){
	}
	std::string nodeKind() override { return "Deref"; }
	virtual void unparse(std::ostream& out, int indent) override;
	virtual bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
protected:
	IDNode * myID;
};

class NegNode : public UnaryExpNode{
public:
	NegNode(Position * p, ExpNode * exp)
	: UnaryExpNode(p, exp){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "Neg"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class NotNode : public UnaryExpNode{
public:
	NotNode(Position * p, ExpNode * exp)
	: UnaryExpNode(p, exp){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "Not"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class VoidTypeNode : public TypeNode{
public:
	VoidTypeNode(Position * p) : TypeNode(p){}
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "VoidType"; }
	virtual const DataType * getType()override { 
		return BasicType::VOID(); 
	}
};

class PtrTypeNode : public TypeNode{
public:
	PtrTypeNode(Position * p, TypeNode * baseTypeIn)
	:TypeNode(p), myBaseType(baseTypeIn) { }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "PTR " + myBaseType->nodeKind(); }
	virtual const DataType * getType() override;
private:
	TypeNode * myBaseType;
};


class IntTypeNode : public TypeNode{
public:
	IntTypeNode(Position * p): TypeNode(p){}
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "IntType"; }
	virtual const DataType * getType() override;
};

class ShortTypeNode : public TypeNode{
public:
	ShortTypeNode(Position * p): TypeNode(p){}
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "ShortType"; }
	virtual const DataType * getType() override { return BasicType::SHORT(); }
};

class BoolTypeNode : public TypeNode{
public:
	BoolTypeNode(Position * p): TypeNode(p) { }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "BoolType"; }
	virtual const DataType * getType() override;
};

class StringTypeNode : public TypeNode{
public:
	StringTypeNode(Position * p): TypeNode(p) { }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "StringType"; }
	virtual const DataType * getType() override;
};

/*
class ArrayTypeNode : public TypeNode{
public:
	ArrayTypeNode(cone_t l, size_t c, TypeNode * base, size_t len): TypeNode(l, c), myLen(len), myBase(base){}
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "StringType"; }
	virtual DataType * getType() override { 
		const BasicType * t = myBase->getType()->asBasic();
		return ArrayType::produce(t, myLen); 
	}
private:
	size_t myLen;
	TypeNode * myBase;
};
*/


class AssignExpNode : public ExpNode{
public:
	AssignExpNode(Position * p, LValNode * dstIn, ExpNode * srcIn)
	: ExpNode(p), myDst(dstIn), mySrc(srcIn){ }
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "AssignExp"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * proc) override;
private:
	LValNode * myDst;
	ExpNode * mySrc;
};

class ShortLitNode : public ExpNode{
public:
	ShortLitNode(Position * p, const int numIn)
	: ExpNode(p), myNum(numIn){ }
	virtual void unparseNested(std::ostream& out) override{
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "ShortLit"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
private:
	const int myNum;
};

class IntLitNode : public ExpNode{
public:
	IntLitNode(Position * p, const int numIn)
	: ExpNode(p), myNum(numIn){ }
	virtual void unparseNested(std::ostream& out) override{
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "IntLit"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
private:
	const int myNum;
};

class StrLitNode : public ExpNode{
public:
	StrLitNode(Position * p, const std::string strIn)
	: ExpNode(p), myStr(strIn){ }
	virtual void unparseNested(std::ostream& out) override{
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "StrLit"; }
	bool nameAnalysis(SymbolTable *) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * proc) override;
private:
	 const std::string myStr;
};



class TrueNode : public ExpNode{
public:
	TrueNode(Position * p): ExpNode(p){ }
	virtual void unparseNested(std::ostream& out) override{
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "True"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class FalseNode : public ExpNode{
public:
	FalseNode(Position * p): ExpNode(p){ }
	virtual void unparseNested(std::ostream& out) override{
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
	virtual std::string nodeKind() override { return "False"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual Opd * flatten(Procedure * prog) override;
};

class CallStmtNode : public StmtNode{
public:
	CallStmtNode(Position * p, CallExpNode * expIn)
	: StmtNode(p), myCallExp(expIn){ }
	void unparse(std::ostream& out, int indent) override;
	std::string nodeKind() override { return "CallStmt"; }
	bool nameAnalysis(SymbolTable * symTab) override;
	virtual void typeAnalysis(TypeAnalysis *) override;
	virtual void to3AC(Procedure * proc) override;
private:
	CallExpNode * myCallExp;
};

} //End namespace cminusminus

#endif

