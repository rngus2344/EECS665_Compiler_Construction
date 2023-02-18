#include <ostream>
#include "3ac.hpp"

namespace cshanty{

map<string, size_t> funcFormalsMap;

void IRProgram::allocGlobals(){
	//Choose a label for each global

	for(auto g: globals) {
		string loc = string("glbl_") + g.first->getName();
		g.second->setMemoryLoc(loc);
	}

	for(auto s: strings) {
		s.first->setMemoryLoc(s.first->valString());
	}
	
}

void IRProgram::datagenX64(std::ostream& out){
	out << ".data" << endl;
	for(auto g: globals) {
		out << g.second->getMemoryLoc() << ":\t" << ".quad 0" <<endl;
	}
	//Put this directive after you write out strings
	// so that everything is aligned to a quadword value
	// again

	for(auto s: strings) {
		out << s.first->valString() << ":\t" << ".asciz " << s.second <<endl;
	}	
	if (!strings.empty())
		out << ".align 8\n";
	
}

void IRProgram::toX64(std::ostream& out){
	allocGlobals();
	datagenX64(out);
	// Iterate over each procedure and codegen it

	out << ".global main" <<endl;
	out << ".text" <<endl;

	for(auto proc : *procs)
		proc->toX64(out);
}

void Procedure::allocLocals(){
	//Allocate space for locals
	// Iterate over each procedure and codegen it
	size_t offset = 16;
	for(auto local: locals) {
		offset += local.second->getWidth();
		string loc = string("-") + std::to_string(offset) + "(%rbp)";
		local.second->setMemoryLoc(loc);
	}
}

void Procedure::allocFormals(){
	//Allocate space for formals
	// Iterate over each procedure and codegen it
	size_t localsOffset = this->localsSize();
	size_t offset =  localsOffset + 16;
	for(auto formal: formals) {
		offset += formal->getWidth();
		string loc = string("-") + std::to_string(offset) + "(%rbp)";

		formal->setMemoryLoc(loc);
	}
}

void Procedure::allocTemps(){
	//Allocate space for temps
	// Iterate over each procedure and codegen it
	size_t localsOffset = this->localsSize();
	size_t formalsOffset = this->formalsSize();
	size_t offset = formalsOffset + localsOffset + 16;
	
	for(auto temp: temps) {
		offset += temp->getWidth();
		string loc = string("-") + std::to_string(offset) + "(%rbp)";
		temp->setMemoryLoc(loc);
	}
}

void Procedure::allocAddrs() {
	//Allocate space for temps
	// Iterate over each procedure and codegen it
	size_t localsOffset = this->localsSize();
	size_t formalsOffset = this->formalsSize();
	size_t tempsOffset = this->tempsSize();
	size_t offset = tempsOffset + formalsOffset + localsOffset + 16;
	
	for(auto addr: this->addrOpds) {
		offset += addr->getWidth();
		string loc = string("-") + std::to_string(offset) + "(%rbp)";
		addr->setMemoryLoc(loc);
	}
}

void Procedure::getArgs(std::ostream& out) {
	size_t index = 1;
	size_t offset  = this->localsSize() + 16;
	for(auto formal: formals) {
		offset += formal->getWidth();
		switch(index) {
			case 1:
				out << "\tmovq %rdi, -" << offset << "(%rbp)" <<endl;
			break;
			case 2:
				out << "\tmovq %rsi, -" << offset << "(%rbp)" <<endl;
			break;
			case 3:
				out << "\tmovq %rdx, -" << offset << "(%rbp)" <<endl;
			break;
			case 4:
				out << "\tmovq %rcx, -" << offset << "(%rbp)" <<endl;
			break;
			case 5:
				out << "\tmovq %r8, -" << offset << "(%rbp)" <<endl;
			break;
			case 6:
				out << "\tmovq %r9, -" << offset << "(%rbp)" <<endl;
			break;	
			default:
				out << "\tmovq " << 8*(formals.size() - index) << "(%rbp)" << ", %r12" << endl;
				out << "\tmovq %r12, -" << offset << "(%rbp)" <<endl; 
			break;	
		}

		index++;
	}
}


void Procedure::toX64(std::ostream& out){
	//Allocate all locals
	allocLocals();
	allocFormals();
	allocTemps();
	allocAddrs();
	enter->codegenLabels(out);
	enter->codegenX64(out);

	size_t sumLocalFormalTempSize = this->arSize();

	if(sumLocalFormalTempSize !=0)
		out << "\tsubq $" << sumLocalFormalTempSize << ",%rsp" <<endl;	

	//keep func Name, formals size map for callq cleanup
	funcFormalsMap[this->getName()] = formals.size();

	//fill all formals 
	getArgs(out);

	out << "#Fn body " << myName << "\n";
	for (auto quad : *bodyQuads){
		quad->codegenLabels(out);
		out << "#" << quad->toString() << "\n";
		quad->codegenX64(out);
	}
	out << "#Fn epilogue " << myName << "\n";
	leave->codegenLabels(out);
	

	// mov rsp back based on locals + formals and temps
	if(sumLocalFormalTempSize !=0 )
		out << "\taddq $" << sumLocalFormalTempSize << ",%rsp" <<endl;

	leave->codegenX64(out);
}

void Quad::codegenLabels(std::ostream& out){
	if (labels.empty()){ return; }

	size_t numLabels = labels.size();
	size_t labelIdx = 0;
	for ( Label * label : labels){
		out << label->getName() << ": ";
		if (labelIdx != numLabels - 1){ out << "\n"; }
		labelIdx++;
	}
}

void BinOpQuad::codegenX64(std::ostream& out){
	switch (this->getOp())
	{
	case BinOp::ADD64:
		src1->genLoadVal(out, Register::A);
		src2->genLoadVal(out, Register::B);
		out << "\taddq %rbx, %rax\n";
		break;
	case BinOp::SUB64:
		src1->genLoadVal(out, Register::A);
		src2->genLoadVal(out, Register::B);
		out << "\tsubq %rbx, %rax\n";
		break;
	case BinOp::MULT64:
		src1->genLoadVal(out, Register::A);
		src2->genLoadVal(out, Register::B);
		out << "\timulq %rbx, %rax\n";
		break;
	case BinOp::DIV64:
		out << "\tmovq $0, %rdx" <<endl;
		src1->genLoadVal(out, Register::A);
		out << "\tcqto" <<endl;
		src2->genLoadVal(out, Register::B);
		out << "\tidivq %rbx\n";
		break;
	case BinOp::OR64:
		src1->genLoadVal(out, Register::A);
		src2->genLoadVal(out, Register::B);
		out << "\torq %rbx, %rax\n";
		break;
	case BinOp::AND64:
		src1->genLoadVal(out, Register::A);
		src2->genLoadVal(out, Register::B);
		out << "\tandq %rbx, %rax\n";
		break;
	case BinOp::EQ64:
		src1->genLoadVal(out, Register::C);
		src2->genLoadVal(out, Register::B);
		out << "\tmovq $0, %rax\n";
		out << "\tcmpq %rbx, %rcx\n";
		out << "\tsete %al\n";
		break;
	// out<<"\taddq $8, %rsp\n";
	case NEQ64:
		src1->genLoadVal(out, Register::C);
		src2->genLoadVal(out, Register::B);
		out << "\tmovq $0, %rax\n";
		out << "\tcmpq %rbx, %rcx\n";
		out << "\tsetne %al\n";
		break;
	case BinOp::LT64:
		src1->genLoadVal(out, Register::A);
		src2->genLoadVal(out, Register::B);
		out << "\tcmpq %rbx, %rax\n";
		out << "\tmovq $0, %rax\n";
		out << "\tsetl %al\n";
		break;
	case BinOp::GT64:
		src1->genLoadVal(out, Register::A);
		src2->genLoadVal(out, Register::B);
		out << "\tcmpq %rbx, %rax\n";
		out << "\tmovq $0, %rax\n";
		out << "\tsetg %al\n";
		break;
	case BinOp::LTE64:
		src1->genLoadVal(out, Register::A);
		src2->genLoadVal(out, Register::B);
		out << "\tcmpq %rbx, %rax\n";
		out << "\tmovq $0, %rax\n";
		out << "\tsetle %al\n";
		break;
	case BinOp::GTE64:
		src1->genLoadVal(out, Register::A);
		src2->genLoadVal(out, Register::B);
		out << "\tcmpq %rbx, %rax\n";
		out << "\tmovq $0, %rax\n";
		out << "\tsetge %al\n";
		break;
	}

	out << "\tmovq %rax," << dst->getMemoryLoc() << endl;

}

void UnaryOpQuad::codegenX64(std::ostream& out){
	switch(this->getOp()) {
		case UnaryOp::NEG64:
		src->genLoadVal(out, Register::A);
		out << "\timulq $-1, %rax\n";
		out << "\tcqto" <<endl;
		break;
		case UnaryOp::NOT64:
		src->genLoadVal(out, Register::A);
		out << "\tcmpq $0, %rax\n";
		out << "\tsete %al\n";		
		break;
	}
	out << "\tmovq %rax," << dst->getMemoryLoc() << endl;
}

void AssignQuad::codegenX64(std::ostream& out){
	src->genLoadVal(out, A);
	dst->genStoreVal(out, A);
}

void GotoQuad::codegenX64(std::ostream& out){
	out << "jmp " << tgt->getName() << "\n";
}

void IfzQuad::codegenX64(std::ostream& out){
	
	out << "\tmovb " << this->getCnd()->getMemoryLoc() << ",%al" <<endl;
	out << "\tcmpb $0,%al" <<endl;
	out << "\tje " << this->getTarget()->getName() << endl;

}

void NopQuad::codegenX64(std::ostream& out){
	out << "nop" << "\n";
}

void IntrinsicOutputQuad::codegenX64(std::ostream& out){
	if (myType->isBool()){
		myArg->genLoadVal(out, DI);
		out << "\tcallq printBool\n";
	} else if(myType->isString()){
		myArg->genLoadVal(out, DI);
		out << "\tcallq printString" << endl;
	} else if(myType->isRecord()) {
		// do nothing, there is no record printing in c lib
	} else if (myType->isInt()) {
		myArg->genLoadVal(out, DI);
		out << "\tcallq printInt" << endl;
	} else {
		//do nothing on void
	}
}

void IntrinsicInputQuad::codegenX64(std::ostream& out){
	if (myType->isBool()) {
		out << "\tcallq getBool\n";
		myArg->genStoreVal(out, A);
	}else if (myType->isInt()) {
		out << "\tcallq getInt\n";
		myArg->genStoreVal(out, A);
	}
}

void CallQuad::codegenX64(std::ostream& out){
	out << "\tcallq fun_" << callee->getName() << endl;

	//stack cleanup for formals that > 6 
	size_t calleeFormalSize = funcFormalsMap[this->callee->getName()];
	if (calleeFormalSize > 6) {
		out <<"\taddq $" << std::to_string( (calleeFormalSize-6) * 8) << ", %rsp" <<endl;
	}
}
	

void EnterQuad::codegenX64(std::ostream& out){
	out << "\n\tpushq %rbp" <<endl;
	out << "\tmovq %rsp,%rbp" <<endl;
	out << "\taddq $16,%rbp" <<endl;

}

void LeaveQuad::codegenX64(std::ostream& out){
	//out << "\taddq $16,%rsp" <<endl;
	out << "\tpopq %rbp" <<endl;
	out << "\tretq" <<endl;
}

void SetArgQuad::codegenX64(std::ostream& out){
	string reg;
	switch(this->getIndex()) {
		case 1: 
		reg = "%rdi";
		break;
		case 2:
		reg = "%rsi";
		break;
		case 3:
		reg = "%rdx";
		break;
		case 4: 
		reg = "%rcx";
		break;
		case 5:
		reg = "%r8";
		break;
		case 6:
		reg = "%r9";
		break;
		default:
		break;
	}
	if(!reg.empty()) {
		if(this->getSrc()->getMemoryLoc() == "UNINIT")
			out << "\tmovq $" << this->getSrc()->valString() << "," << reg << endl;
		else
			out << "\tmovq " << this->getSrc()->getMemoryLoc() << "," << reg << endl;
	}
	
	if(this->getIndex() > 6) {

		if(this->getSrc()->getMemoryLoc() == "UNINIT")
			out << "\tmovq $" << this->getSrc()->valString() << "," << "%r12" << endl;
		else
			out << "\tmovq " << this->getSrc()->getMemoryLoc() << "," << "%r12" << endl;
		out << "\tpushq %r12" <<endl;
	}

}

void GetArgQuad::codegenX64(std::ostream& out){
	//We don't actually need to do anything here
}

void SetRetQuad::codegenX64(std::ostream& out){
	string loc = this->getSrc()->getMemoryLoc();
	if(loc == "UNINIT")
		out << "\tmovq $" << this->getSrc()->valString() << ",%rax" <<endl;
	else
		out << "\tmovq " << this->getSrc()->getMemoryLoc() << ",%rax" <<endl;
}

void GetRetQuad::codegenX64(std::ostream& out){
	out << "\tmovq %rax, " << this->getDst()->getMemoryLoc() << endl;
}

void IndexQuad::codegenX64(std::ostream& out){
	this->src->genLoadAddr(out, A);
	out << "\taddq $" << this->off->valString() << ",%rax" <<endl;
	// out << "\tmovq %rax, " << this->dst->getMemoryLoc() << endl; //same as following line
	this->dst->genStoreAddr(out, A);

}

void SymOpd::genLoadVal(std::ostream& out, Register reg){
	out << "\tmovq " << this->getMemoryLoc() << "," << RegUtils::reg64(reg) <<endl;
}

void SymOpd::genStoreVal(std::ostream& out, Register reg){
	out << "\tmovq " << RegUtils::reg64(reg) << "," << this->getMemoryLoc() << endl;
}

void SymOpd::genLoadAddr(std::ostream& out, Register reg) {
	out << "\tleaq " << this->getMemoryLoc() << "," <<RegUtils::reg64(reg) <<endl;
}

void AuxOpd::genLoadVal(std::ostream& out, Register reg){
	out << "\tmovq " << this->getMemoryLoc() << "," << RegUtils::reg64(reg) <<endl;
}

void AuxOpd::genStoreVal(std::ostream& out, Register reg){
	TODO(Implement me)
}
void AuxOpd::genLoadAddr(std::ostream& out, Register reg){
	TODO(Implement me)
}


void AddrOpd::genStoreVal(std::ostream& out, Register reg){
	out << "\tmovq " << this->getMemoryLoc() << ", %r12" <<endl;
	out << "\tmovq " << RegUtils::reg64(reg) << ",(%r12)" <<endl;
}

void AddrOpd::genLoadVal(std::ostream& out, Register reg){
	out << "\tmovq " << this->getMemoryLoc() << ", %rax" <<endl;
	out << "\tmovq (%rax), " << RegUtils::reg64(reg) <<endl;
}

void AddrOpd::genStoreAddr(std::ostream& out, Register reg){
	out << "\tmovq " << RegUtils::reg64(reg) <<  "," << this->getMemoryLoc() << endl;
}

void AddrOpd::genLoadAddr(std::ostream & out, Register reg){
	TODO(Implement me)
}

void LitOpd::genLoadVal(std::ostream & out, Register reg){
	out << "\t" << getMovOp() << " $" << val << ", " << getReg(reg) << "\n";
}

}
