#include <ostream>
#include "3ac.hpp"

namespace cminusminus{

void IRProgram::allocGlobals() {
	//Choose a label for each global
	for(auto g: globals) {
		SymOpd * globalOpd = g.second;
		std::string memLoc = "gbl_";
		const SemSymbol * sym = globalOpd->getSym();
		memLoc += sym->getName();
		globalOpd->setMemoryLoc("(" + memLoc + ")");
	}

	for(auto s: strings) {
		s.first->setMemoryLoc(s.first->valString());
	}
}

void IRProgram::datagenX64(std::ostream& out) {
	out << ".data\n";
	out << ".globl main\n";
	for(auto g: globals) {
		SymOpd * globalOpd = g.second;
		std::string memLoc = "gbl_";
		const SemSymbol * sym = globalOpd->getSym();
		memLoc += sym->getName();
		size_t width = sym->getDataType()->getSize();
		out << memLoc << ": ";
		if (width == 8) {
			out << ".quad 0\n";
		} else {
			out << ".space " << width << "\n";
		}
	}
	//Put this directive after you write out strings
	// so that everything is aligned to a quadword value
	// again
	for(auto s: strings) {
		out << s.first->valString() << ":\t.asciz " << s.second << "\n";
	}	
	if (!strings.empty())
		out << ".align 8\n";	
}

void IRProgram::toX64(std::ostream& out) {
	allocGlobals();
	datagenX64(out);
	// Iterate over each procedure and codegen it
	out << ".text\n";

	for(auto proc : *procs)
		proc->toX64(out);
}

void Procedure::allocLocals() {
	//Allocate space for locals
	// Iterate over each procedure and codegen it
	size_t offset = 16;
	size_t localsOffset = this->localsSize();
	size_t formalsOffset = this->formalsSize();
	size_t tempsOffset = this->tempsSize();
	for(auto local: locals) {
		offset += local.second->getWidth();
		string loc = string("-") + std::to_string(offset) + "(%rbp)";
		local.second->setMemoryLoc(loc);
	}
	offset = localsOffset + 16;
	for(auto formal: formals) {
		offset += formal->getWidth();
		string loc = string("-") + std::to_string(offset) + "(%rbp)";

		formal->setMemoryLoc(loc);
	}
	offset = formalsOffset + localsOffset + 16;
	for(auto temp: temps) {
		offset += temp->getWidth();
		string loc = string("-") + std::to_string(offset) + "(%rbp)";
		temp->setMemoryLoc(loc);
	}
	offset = tempsOffset + formalsOffset + localsOffset + 16;
	for(auto addr: this->addrOpds) {
		offset += addr->getWidth();
		string loc = string("-") + std::to_string(offset) + "(%rbp)";
		addr->setMemoryLoc(loc);
	}
}

void Procedure::toX64(std::ostream& out) {
	//Allocate all locals
	allocLocals();

	enter->codegenLabels(out);
	enter->codegenX64(out);
	out << "#Fn body " << myName << "\n";
	for (auto quad : *bodyQuads) {
		quad->codegenLabels(out);
		out << "#" << quad->toString() << "\n";
		quad->codegenX64(out);
	}
	out << "#Fn epilogue " << myName << "\n";
	leave->codegenLabels(out);
	leave->codegenX64(out);
}

void Quad::codegenLabels(std::ostream& out) {
	if (labels.empty()) { return; }

	size_t numLabels = labels.size();
	size_t labelIdx = 0;

	for (Label * label : labels) {
		out << label->getName() << ": ";
		if (labelIdx != numLabels - 1) { out << "\n"; }
		labelIdx++;
	}
}

void BinOpQuad::codegenX64(std::ostream& out) {
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
		case BinOp::DIV64:
			out << "\tmovq $0, %rdx\n";
			src1->genLoadVal(out, Register::A);
			out << "\tcqto\n";
			src2->genLoadVal(out, Register::B);
			out << "\tidivq %rbx\n";
			break;
		case BinOp::MULT64:
			src1->genLoadVal(out, Register::A);
			src2->genLoadVal(out, Register::B);
			out << "\timulq %rbx, %rax\n";
			break;
		case BinOp::EQ64:
			src1->genLoadVal(out, Register::C);
			src2->genLoadVal(out, Register::B);
			out << "\tmovq $0, %rax\n";
			out << "\tcmpq %rbx, %rcx\n";
			out << "\tsete %al\n";
			break;
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

		case BinOp::ADD8:
			src1->genLoadVal(out, Register::A);
			src2->genLoadVal(out, Register::B);
			out << "\taddq %rbx, %rax\n";
			break;
		case BinOp::SUB8:
			src1->genLoadVal(out, Register::A);
			src2->genLoadVal(out, Register::B);
			out << "\tsubq %rbx, %rax\n";
			break;
		case BinOp::DIV8:
			out << "\tmovq $0, %rdx\n";
			src1->genLoadVal(out, Register::A);
			out << "\tcqto\n";
			src2->genLoadVal(out, Register::B);
			out << "\tidivq %rbx\n";
			break;
		case BinOp::MULT8:
			src1->genLoadVal(out, Register::A);
			src2->genLoadVal(out, Register::B);
			out << "\timulq %rbx, %rax\n";
			break;
		case BinOp::EQ8:
			src1->genLoadVal(out, Register::C);
			src2->genLoadVal(out, Register::B);
			out << "\tmovq $0, %rax\n";
			out << "\tcmpq %rbx, %rcx\n";
			out << "\tsete %al\n";
			break;
		case NEQ8:
			src1->genLoadVal(out, Register::C);
			src2->genLoadVal(out, Register::B);
			out << "\tmovq $0, %rax\n";
			out << "\tcmpq %rbx, %rcx\n";
			out << "\tsetne %al\n";
			break;
		case BinOp::LT8:
			src1->genLoadVal(out, Register::A);
			src2->genLoadVal(out, Register::B);
			out << "\tcmpq %rbx, %rax\n";
			out << "\tmovq $0, %rax\n";
			out << "\tsetl %al\n";
			break;
		case BinOp::GT8:
			src1->genLoadVal(out, Register::A);
			src2->genLoadVal(out, Register::B);
			out << "\tcmpq %rbx, %rax\n";
			out << "\tmovq $0, %rax\n";
			out << "\tsetg %al\n";
			break;
		case BinOp::LTE8:
			src1->genLoadVal(out, Register::A);
			src2->genLoadVal(out, Register::B);
			out << "\tcmpq %rbx, %rax\n";
			out << "\tmovq $0, %rax\n";
			out << "\tsetle %al\n";
			break;
		case BinOp::GTE8:
			src1->genLoadVal(out, Register::A);
			src2->genLoadVal(out, Register::B);
			out << "\tcmpq %rbx, %rax\n";
			out << "\tmovq $0, %rax\n";
			out << "\tsetge %al\n";
			break;
		case BinOp::OR8:
			src1->genLoadVal(out, Register::A);
			src2->genLoadVal(out, Register::B);
			out << "\torq %rbx, %rax\n";
			break;
		case BinOp::AND8:
			src1->genLoadVal(out, Register::A);
			src2->genLoadVal(out, Register::B);
			out << "\tandq %rbx, %rax\n";
			break;
	}

	out << "\tmovq %rax, " << dst->getMemoryLoc() << "\n";
}

void UnaryOpQuad::codegenX64(std::ostream& out) {
	switch(this->getOp()) {
		case UnaryOp::NEG64:
			src->genLoadVal(out, Register::A);
			out << "\timulq $-1, %rax\n";
			out << "\tcqto\n";
			break;
		case UnaryOp::NOT64:
			src->genLoadVal(out, Register::A);
			out << "\tcmpq $0, %rax\n";
			out << "\tsete %al\n";		
			break;
		case UnaryOp::NEG8:
			src->genLoadVal(out, Register::A);
			out << "\timulq $-1, %rax\n";
			out << "\tcqto\n";
			break;
		case UnaryOp::NOT8:
			src->genLoadVal(out, Register::A);
			out << "\tcmpq $0, %rax\n";
			out << "\tsete %al\n";		
			break;
	}
	out << "\tmovq %rax, " << dst->getMemoryLoc() << "\n";
}

void AssignQuad::codegenX64(std::ostream& out) {
	src->genLoadVal(out, A);
	dst->genStoreVal(out, A);
}

void GotoQuad::codegenX64(std::ostream& out) {
	out << "jmp " << tgt->getName() << "\n";
}

void IfzQuad::codegenX64(std::ostream& out) {
	out << "\tmovb " << this->getCnd()->getMemoryLoc() << ", %al\n";
	out << "\tcmpb $0, %al\n";
	out << "\tje " << this->getTarget()->getName() << "\n";
}

void NopQuad::codegenX64(std::ostream& out) {
	out << "\tnop\n";
}

void IntrinsicOutputQuad::codegenX64(std::ostream& out) {
	if(myType->isBool()) {
		myArg->genLoadVal(out, DI);
		out << "\tcallq printBool\n";
	} else if(myType->isString()) {
		myArg->genLoadVal(out, DI);
		out << "\tcallq printString\n";
	} else if(myType->isShort()) {
		myArg->genLoadVal(out, DI);
		out << "\tcallq printShort\n";
	} else if(myType->isPtr()) {
		myArg->genLoadVal(out, DI);
		out << "\tcallq printPtr\n";
	} else if(myType->isInt()) {
		myArg->genLoadVal(out, DI);
		out << "\tcallq printInt\n";
	} else {
	}
}

void IntrinsicInputQuad::codegenX64(std::ostream& out) {
	if(myType->isBool()) {
		out << "\tcallq getBool\n";
		myArg->genStoreVal(out, A);
	} else if(myType->isInt()) {
		out << "\tcallq getInt\n";
		myArg->genStoreVal(out, A);
	} else if(myType->isString()) {
		out << "\tcallq getString\n";
		myArg->genStoreVal(out, A);
	} else if(myType->isShort()) {
		out << "\tcallq getShort\n";
		myArg->genStoreVal(out, A);
	} else if(myType->isPtr()) {
		out << "\tcallq getPtr\n";
		myArg->genStoreVal(out, A);
	}
}

void CallQuad::codegenX64(std::ostream& out) {
	out << "\tcallq fun_" << callee->getName() << "\n";

	map<string, size_t> fMap;

	size_t calleeFormalSize = fMap[this->callee->getName()];
	if (calleeFormalSize > 6) {
		out <<"\taddq $" << std::to_string( (calleeFormalSize-6) * 8) << ", %rsp\n";
	}
}

void EnterQuad::codegenX64(std::ostream& out) {
	out << "\n\tpushq %rbp\n";
	out << "\tmovq %rsp, %rbp\n";
	out << "\taddq $16, %rbp\n";
}

void LeaveQuad::codegenX64(std::ostream& out) {
	out << "\tpopq %rbp\n";
	out << "\tretq\n";
}

void SetArgQuad::codegenX64(std::ostream& out) {
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
			out << "\tmovq $" << this->getSrc()->valString() << ", " << reg << "\n";
		else
			out << "\tmovq " << this->getSrc()->getMemoryLoc() << ", " << reg << "\n";
	}
	
	if(this->getIndex() > 6) {

		if(this->getSrc()->getMemoryLoc() == "UNINIT")
			out << "\tmovq $" << this->getSrc()->valString() << ", %r12\n";
		else
			out << "\tmovq " << this->getSrc()->getMemoryLoc() << ", %r12\n";
		out << "\tpushq %r12\n";
	}
}

void GetArgQuad::codegenX64(std::ostream& out) {
	//We don't actually need to do anything here
}

void SetRetQuad::codegenX64(std::ostream& out) {
	string loc = this->getSrc()->getMemoryLoc();
	if(loc == "UNINIT")
		out << "\tmovq $" << this->getSrc()->valString() << ", %rax\n";
	else
		out << "\tmovq " << this->getSrc()->getMemoryLoc() << ", %rax\n";
}

void GetRetQuad::codegenX64(std::ostream& out) {
	out << "\tmovq %rax, " << this->getDst()->getMemoryLoc() << "\n";
}

void LocQuad::codegenX64(std::ostream& out) {
	out << "\tmovq %rax, " << this->getSrc()->getMemoryLoc() << "\n";
}

void SymOpd::genLoadVal(std::ostream& out, Register reg) {
	out << "\tmovq " << this->getMemoryLoc() << ", " << RegUtils::reg64(reg) << "\n";
}

void SymOpd::genStoreVal(std::ostream& out, Register reg) {
	out << "\tmovq " << RegUtils::reg64(reg) << ", " << this->getMemoryLoc() << "\n";
}

void SymOpd::genLoadAddr(std::ostream& out, Register reg) {
	// TODO(Implement me if necessary)
	out << "\tleaq " << this->getMemoryLoc() << ", " <<RegUtils::reg64(reg) << "\n";
}

void AuxOpd::genLoadVal(std::ostream& out, Register reg) {
	out << "\tmovq " << this->getMemoryLoc() << ", " << RegUtils::reg64(reg) << "\n";
}

void AuxOpd::genStoreVal(std::ostream& out, Register reg) {
	out << "\tmovq " << RegUtils::reg64(reg) << ", " << this->getMemoryLoc() << "\n";
}
void AuxOpd::genLoadAddr(std::ostream& out, Register reg) {
	out << "\tleaq " << this->getMemoryLoc() << ", " <<RegUtils::reg64(reg) << "\n";
}


void AddrOpd::genStoreVal(std::ostream& out, Register reg) {
	out << "\tmovq " << this->getMemoryLoc() << ", %r12\n";
	out << "\tmovq " << RegUtils::reg64(reg) << ", (%r12)\n";
}

void AddrOpd::genLoadVal(std::ostream& out, Register reg) {
	out << "\tmovq " << this->getMemoryLoc() << ", %rax\n";
	out << "\tmovq (%rax), " << RegUtils::reg64(reg) << "\n";
}

void AddrOpd::genStoreAddr(std::ostream& out, Register reg) {
	out << "\tmovq " << RegUtils::reg64(reg) <<  ", " << this->getMemoryLoc() << "\n";
}

void AddrOpd::genLoadAddr(std::ostream & out, Register reg) {
	out << "\tleaq " << this->getMemoryLoc() << ", " <<RegUtils::reg64(reg) << "\n";
}

void LitOpd::genLoadVal(std::ostream & out, Register reg) {
	out << "\t" << getMovOp() << " $" << val << ", " << getReg(reg) << "\n";
}

}
