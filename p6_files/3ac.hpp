#ifndef CMINUSMINUS_3AC_HPP
#define CMINUSMINUS_3AC_HPP

#include <assert.h>
#include <list>
#include <map>
#include <set>
#include <string.h>
#include "symbol_table.hpp"
#include "types.hpp"

namespace cminusminus{

class TypeAnalysis;
class Procedure;
class IRProgram;

class Label{
public:
	Label(std::string nameIn){
		this->name = nameIn;
	}
	std::string getName(){
		return name;
	}
private:
	std::string name;
};

class Opd{
public:
	Opd(size_t widthIn) : myWidth(widthIn){}
	virtual std::string valString() = 0;
	virtual std::string locString() = 0;
	virtual size_t getWidth(){ return myWidth; }
	static size_t width(const DataType * type){
		if (const BasicType * basic = type->asBasic()){
			return 8;
		}
		assert(false);
	}
private:
	size_t myWidth;
};

class SymOpd : public Opd{
public:
	virtual std::string valString() override{
		return "[" + mySym->getName() + "]";
	}
	virtual std::string locString() override{
		return mySym->getName();
	}
	virtual std::string getName(){
		return mySym->getName();
	}
	const SemSymbol * getSym(){ return mySym; }
private:
	//Private Constructor
	SymOpd(SemSymbol * sym, size_t width)
	: Opd(width), mySym(sym) {} 
	SemSymbol * mySym;
	friend class Procedure;
	friend class IRProgram;
};

class LitOpd : public Opd{
public:
	LitOpd(std::string valIn, size_t width)
	: Opd(width), val(valIn){ }
	virtual std::string valString() override { return val; }
	virtual std::string locString() override { 
		throw InternalError("Tried to get location of a constant");
	}
private:
	std::string val;
	std::string name;
};

class AuxOpd : public Opd{
public:
	AuxOpd(std::string nameIn, size_t width) 
	: Opd(width), name(nameIn) { }
	virtual std::string valString() override{
		return "[" + getName() + "]";
	}
	virtual std::string locString() override{
		return getName();
	}
	std::string getName(){
		return name;
	}
private:
	std::string name;
};

class AddrOpd : public Opd{
public:
	AddrOpd(std::string nameIn, size_t width)
	: Opd(width), name(nameIn) { }
	virtual std::string valString() override{
		return "[" + getName() + "]";
	}
	virtual std::string locString() override{
		return getName();
	}
	virtual std::string getName(){
		return name;
	}
private:
	std::string val;
	std::string name;
};

enum BinOp {
	ADD64, SUB64, DIV64, MULT64, EQ64, NEQ64, LT64, GT64, LTE64, GTE64, AND64, OR64
};
enum UnaryOp{
	NEG64, NOT8
};

class Quad{
public:
	Quad();
	void addLabel(Label * label);
	Label * getLabel(){ return labels.front(); }
	virtual std::string repr() = 0;
	std::string commentStr();
	virtual std::string toString(bool verbose=false);
	void setComment(std::string commentIn);
private:
	std::string myComment;
	std::list<Label *> labels;
};

class BinOpQuad : public Quad{
public:
	BinOpQuad(Opd * dstIn, BinOp oprIn, Opd * src1In, Opd * src2In);
	std::string repr() override;
	static std::string oprString(BinOp opr);
private:
	Opd * dst;
	BinOp opr;
	Opd * src1;
	Opd * src2;
};

class UnaryOpQuad : public Quad {
public:
	UnaryOpQuad(Opd * dstIn, UnaryOp opIn, Opd * srcIn);
	std::string repr() override ;
	Opd * getDst(){ return dst; }
	Opd * getSrc(){ return src; }
	UnaryOp getOp(){ return op; }
private:
	Opd * dst;
	UnaryOp op;
	Opd * src;
};

class AssignQuad : public Quad{
	
public:
	AssignQuad(Opd * dstIn, Opd * srcIn);
	std::string repr() override;
	Opd * getDst(){ return dst; }
	Opd * getSrc(){ return src; }
private:
	Opd * dst;
	Opd * src;
};

class LocQuad : public Quad {
public:
	LocQuad(Opd * srcIn, Opd * tgtIn, bool srcLocIn, bool tgtLocIn)
	: src(srcIn), tgt(tgtIn), srcIsLoc(srcLocIn), tgtIsLoc(tgtLocIn){ }
	std::string repr() override;
private:
	Opd * src;
	Opd * tgt;
	bool srcIsLoc;
	bool tgtIsLoc;
};


class GotoQuad : public Quad {
public:
	GotoQuad(Label * tgtIn);
	std::string repr() override;
	Label * getTarget(){ return tgt; }
private:
	Label * tgt;
};

class IfzQuad : public Quad {
public:
	IfzQuad(Opd * cndIn, Label * tgtIn);
	std::string repr() override;
	Label * getTarget(){ return tgt; }
	Opd * getCnd(){ return cnd; }
private:
	Opd * cnd;
	Label * tgt;
};

class NopQuad : public Quad {
public:
	NopQuad();
	std::string repr() override;
};

class ReportQuad : public Quad {
public:
	ReportQuad(Opd * arg, const DataType * type);
	std::string repr() override;
	Opd * getSrc(){ return myArg; }
	const DataType * getType(){ return myType; }
private:
	Opd * myArg;
	const DataType * myType;
};

class ReceiveQuad : public Quad {
public:
	ReceiveQuad(Opd * arg, const DataType * type);
	std::string repr() override;
	Opd * getDst(){ return myArg; }
private:
	Opd * myArg;
	const DataType * myType;
};

class CallQuad : public Quad{
public:
	CallQuad(SemSymbol * calleeIn);
	std::string repr() override;
private:
	SemSymbol * callee;
};

class EnterQuad : public Quad{
public:
	EnterQuad(Procedure * proc);
	virtual std::string repr() override;
private:
	Procedure * myProc;
};

class LeaveQuad : public Quad{
public:
	LeaveQuad(Procedure * proc);
	virtual std::string repr() override;
private:
	Procedure * myProc;
};

class SetArgQuad : public Quad{
public:
	SetArgQuad(size_t indexIn, Opd * opdIn);
	std::string repr() override;
private:
	size_t index;
	Opd * opd;
};

class GetArgQuad : public Quad{
public:
	GetArgQuad(size_t indexIn, Opd * opdIn);
	std::string repr() override;
	Opd * getDst(){ return opd; }
private:
	size_t index;
	Opd * opd;
};

class SetRetQuad : public Quad{
public:
	SetRetQuad(Opd * opdIn);
	std::string repr() override;
	Opd * getSrc(){ return opd; }
private:
	Opd * opd;
};

class GetRetQuad : public Quad{
public:
	GetRetQuad(Opd * opdIn);
	std::string repr() override;
	Opd * getDst(){ return opd; }
private:
	Opd * opd;
};

class Procedure{
public:
	Procedure(IRProgram * prog, std::string name);
	void addQuad(Quad * quad);
	Quad * popQuad();
	IRProgram * getProg();
	std::list<SymOpd *> getFormals() { return formals; }
	SymOpd * getFormal(size_t idx){
		auto itr = formals.begin();
		for (size_t i = 0; i < idx; i++, itr++);
		return *itr;
	}
	cminusminus::Label * makeLabel();

	void gatherLocal(SemSymbol * sym);
	void gatherFormal(SemSymbol * sym);
	SymOpd * getSymOpd(SemSymbol * sym);
	AuxOpd * makeTmp(size_t width);
	AddrOpd * makeAddrOpd(size_t width);

	std::string toString(bool verbose=false); 
	std::string getName();

	cminusminus::Label * getLeaveLabel();
private:
	EnterQuad * enter;
	LeaveQuad * leave;
	Label * leaveLabel;

	IRProgram * myProg;
	std::map<SemSymbol *, SymOpd *> locals;
	std::list<AuxOpd *> temps; 
	std::list<SymOpd *> formals; 
	std::list<AddrOpd *> addrOpds;
	std::list<Quad *> * bodyQuads;
	std::string myName;
	size_t maxTmp;
};

class IRProgram{
public:
	IRProgram(TypeAnalysis * taIn) : ta(taIn){
		procs = new std::list<Procedure *>();
	}
	Procedure * makeProc(std::string name);
	std::list<Procedure *> * getProcs();
	Label * makeLabel();
	Opd * makeString(std::string val);
	void gatherGlobal(SemSymbol * sym);
	SymOpd * getGlobal(SemSymbol * sym);
	size_t opWidth(ASTNode * node);
	const DataType * nodeType(ASTNode * node);
	std::set<Opd *> globalSyms();

	std::string toString(bool verbose=false);
private:
	TypeAnalysis * ta;
	size_t max_label = 0;
	size_t str_idx = 0;
	std::list<Procedure *> * procs; 
	HashMap<AddrOpd *, std::string> strings;
	std::map<SemSymbol *, SymOpd *> globals;
};

}

#endif 
