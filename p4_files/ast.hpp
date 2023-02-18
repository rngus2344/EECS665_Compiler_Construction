#ifndef CMINUSMINUS_AST_HPP
#define CMINUSMINUS_AST_HPP

#include <ostream>
#include <sstream>
#include <string.h>
#include <list>
#include "tokens.hpp"

namespace cminusminus {

class NameAnalysis;

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
	virtual bool nameAnalysis(SymbolTable *);
protected:
	Position * myPos = nullptr;
};

class ProgramNode : public ASTNode{
public:
	ProgramNode(std::list<DeclNode *> * globalsIn);
	void unparse(std::ostream&, int) override;
	virtual bool nameAnalysis(SymbolTable *) override;
private:
	std::list<DeclNode *> * myGlobals;
};

class ExpNode : public ASTNode{
protected:
	ExpNode(Position * p) : ASTNode(p){ }
public:
	virtual void unparseNested(std::ostream& out);
};

class LValNode : public ExpNode{
public:
	LValNode(Position * p) : ExpNode(p){}
	void unparse(std::ostream& out, int indent) override = 0;
	void unparseNested(std::ostream& out) override;
	void attachSymbol(SemSymbol * symbolIn) { } 
	bool nameAnalysis(SymbolTable * symTab) override { return false; }
};

class IDNode : public LValNode{
public:
	IDNode(Position * p, std::string nameIn)
	: LValNode(p), name(nameIn), mySymbol(nullptr){}
	std::string getName(){ return name; }
	void unparse(std::ostream& out, int indent) override;
	void attachSymbol(SemSymbol * symbolIn);
	SemSymbol * getSymbol() const { return mySymbol; }
private:
	std::string name;
	SemSymbol * mySymbol;
};

class TypeNode : public ASTNode{
public:
	TypeNode(Position * p) : ASTNode(p){ }
	void unparse(std::ostream&, int) override = 0;
};

class StmtNode : public ASTNode{
public:
	StmtNode(Position * p) : ASTNode(p){ }
	virtual void unparse(std::ostream& out, int indent) override = 0;
};

class DeclNode : public StmtNode{
public:
	DeclNode(Position * p) : StmtNode(p){ }
	void unparse(std::ostream& out, int indent) override =0;
};

class VarDeclNode : public DeclNode{
public:
	VarDeclNode(Position * p, TypeNode * typeIn, IDNode * IDIn)
	: DeclNode(p), myType(typeIn), myID(IDIn){ }
	void unparse(std::ostream& out, int indent) override;
	IDNode * ID(){ return myID; }
	TypeNode * getTypeNode(){ return myType; }
	bool nameAnalysis(SymbolTable * symTab) override;
private:
	TypeNode * myType;
	IDNode * myID;
};

class FormalDeclNode : public VarDeclNode{
public:
	FormalDeclNode(Position * p, TypeNode * type, IDNode * id) 
	: VarDeclNode(p, type, id){ }
	void unparse(std::ostream& out, int indent) override;
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
	virtual bool nameAnalysis(SymbolTable * symTab) override;
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
private:
	AssignExpNode * myExp;
};

class ReadStmtNode : public StmtNode{
public:
	ReadStmtNode(Position * p, LValNode * dstIn)
	: StmtNode(p), myDst(dstIn){ }
	void unparse(std::ostream& out, int indent) override;
private:
	LValNode * myDst;
};

class WriteStmtNode : public StmtNode{
public:
	WriteStmtNode(Position * p, ExpNode * srcIn)
	: StmtNode(p), mySrc(srcIn){ }
	void unparse(std::ostream& out, int indent) override;
private:
	ExpNode * mySrc;
};

class PostDecStmtNode : public StmtNode{
public:
	PostDecStmtNode(Position * p, LValNode * lvalIn)
	: StmtNode(p), myLVal(lvalIn){ }
	void unparse(std::ostream& out, int indent) override;
private:
	LValNode * myLVal;
};

class PostIncStmtNode : public StmtNode{
public:
	PostIncStmtNode(Position * p, LValNode * lvalIn)
	: StmtNode(p), myLVal(lvalIn){ }
	void unparse(std::ostream& out, int indent) override;
private:
	LValNode * myLVal;
};

class IfStmtNode : public StmtNode{
public:
	IfStmtNode(Position * p, ExpNode * condIn,
	  std::list<StmtNode *> * bodyIn)
	: StmtNode(p), myCond(condIn), myBody(bodyIn){ }
	void unparse(std::ostream& out, int indent) override;
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
private:
	ExpNode * myCond;
	std::list<StmtNode *> * myBody;
};

class ReturnStmtNode : public StmtNode{
public:
	ReturnStmtNode(Position * p, ExpNode * exp)
	: StmtNode(p), myExp(exp){ }
	void unparse(std::ostream& out, int indent) override;
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
private:
	IDNode * myID;
	std::list<ExpNode *> * myArgs;
};

class BinaryExpNode : public ExpNode{
public:
	BinaryExpNode(Position * p, ExpNode * lhs, ExpNode * rhs)
	: ExpNode(p), myExp1(lhs), myExp2(rhs) { }
protected:
	ExpNode * myExp1;
	ExpNode * myExp2;
};

class PlusNode : public BinaryExpNode{
public:
	PlusNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class MinusNode : public BinaryExpNode{
public:
	MinusNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class TimesNode : public BinaryExpNode{
public:
	TimesNode(Position * p, ExpNode * e1In, ExpNode * e2In)
	: BinaryExpNode(p, e1In, e2In){ }
	void unparse(std::ostream& out, int indent) override;
};

class DivideNode : public BinaryExpNode{
public:
	DivideNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class AndNode : public BinaryExpNode{
public:
	AndNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class OrNode : public BinaryExpNode{
public:
	OrNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class EqualsNode : public BinaryExpNode{
public:
	EqualsNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class NotEqualsNode : public BinaryExpNode{
public:
	NotEqualsNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class LessNode : public BinaryExpNode{
public:
	LessNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class LessEqNode : public BinaryExpNode{
public:
	LessEqNode(Position * pos, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(pos, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class GreaterNode : public BinaryExpNode{
public:
	GreaterNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class GreaterEqNode : public BinaryExpNode{
public:
	GreaterEqNode(Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class UnaryExpNode : public ExpNode {
public:
	UnaryExpNode(Position * p, ExpNode * expIn) 
	: ExpNode(p){
		this->myExp = expIn;
	}
	virtual void unparse(std::ostream& out, int indent) override = 0;
protected:
	ExpNode * myExp;
};

class RefNode : public UnaryExpNode{
public:
	RefNode(Position * p, IDNode * IDIn) 
	: UnaryExpNode(p, IDIn), myID(IDIn){
	}
	virtual void unparse(std::ostream& out, int indent) override;
protected:
	IDNode * myID;
};

class DerefNode : public LValNode{
public:
	DerefNode(Position * p, IDNode * IDIn) 
	: LValNode(p), myID(IDIn){
	}
	virtual void unparse(std::ostream& out, int indent) override;
protected:
	IDNode * myID;
};

class NegNode : public UnaryExpNode{
public:
	NegNode(Position * p, ExpNode * exp)
	: UnaryExpNode(p, exp){ }
	void unparse(std::ostream& out, int indent) override;
};

class NotNode : public UnaryExpNode{
public:
	NotNode(Position * p, ExpNode * exp)
	: UnaryExpNode(p, exp){ }
	void unparse(std::ostream& out, int indent) override;
};

class VoidTypeNode : public TypeNode{
public:
	VoidTypeNode(Position * p) : TypeNode(p){}
	void unparse(std::ostream& out, int indent) override;
};

class PtrTypeNode : public TypeNode{
public:
	PtrTypeNode(Position * p, TypeNode * baseTypeIn)
	:TypeNode(p), myBaseType(baseTypeIn) { }
	void unparse(std::ostream& out, int indent) override;
private:
	TypeNode * myBaseType;
};


class IntTypeNode : public TypeNode{
public:
	IntTypeNode(Position * p): TypeNode(p){}
	void unparse(std::ostream& out, int indent) override;
	bool nameAnalysis(SymbolTable *) override;
};

class ShortTypeNode : public TypeNode{
public:
	ShortTypeNode(Position * p): TypeNode(p){}
	void unparse(std::ostream& out, int indent) override;
};

class BoolTypeNode : public TypeNode{
public:
	BoolTypeNode(Position * p): TypeNode(p) { }
	void unparse(std::ostream& out, int indent) override;
};

class StringTypeNode : public TypeNode{
public:
	StringTypeNode(Position * p): TypeNode(p) { }
	void unparse(std::ostream& out, int indent) override;
};

class AssignExpNode : public ExpNode{
public:
	AssignExpNode(Position * p, LValNode * dstIn, ExpNode * srcIn)
	: ExpNode(p), myDst(dstIn), mySrc(srcIn){ }
	void unparse(std::ostream& out, int indent) override;
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
};

class FalseNode : public ExpNode{
public:
	FalseNode(Position * p): ExpNode(p){ }
	virtual void unparseNested(std::ostream& out) override{
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
};

class CallStmtNode : public StmtNode{
public:
	CallStmtNode(Position * p, CallExpNode * expIn)
	: StmtNode(p), myCallExp(expIn){ }
	void unparse(std::ostream& out, int indent) override;
private:
	CallExpNode * myCallExp;
};

} //End namespace cminusminus

#endif

