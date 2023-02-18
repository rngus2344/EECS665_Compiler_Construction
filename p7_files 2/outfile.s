.data
.globl main
.text
fun_f: 
	pushq %rbp
	movq %rsp, %rbp
	addq $16, %rbp
#Fn body f
#            getarg 1 [num1]
#            getarg 2 [num2]
#            [tmp0] := [num1] ADD64 [num2]
	movq -24(%rbp), %rax
	movq -32(%rbp), %rbx
	addq %rbx, %rax
	movq %rax, -40(%rbp)
#            setret [tmp0]
	movq -40(%rbp), %rax
#            goto lbl_0
jmp lbl_0
#Fn epilogue f
lbl_0: 	popq %rbp
	retq
main: 
	pushq %rbp
	movq %rsp, %rbp
	addq $16, %rbp
#Fn body main
#            [tmp0] := a
	movq %rax, -24(%rbp)
#            [b] := [tmp0]
	movq -48(%rbp), %rax
	movq %rax, -32(%rbp)
#            [addrTmp1] := [b]
	movq %rax, -32(%rbp)
#            [[addrTmp1]] := 4
	movq $4, %rax
	movq -64(%rbp), %r12
	movq %rax, (%r12)
#            [a] := 1
	movq $1, %rax
	movq %rax, -24(%rbp)
#            [c] := 1
	movq $1, %rax
	movq %rax, -40(%rbp)
#            setarg 1 [a]
	movq -24(%rbp), %rdi
#            setarg 2 [c]
	movq -40(%rbp), %rsi
#            call f
	callq fun_f
#            getret [tmp2]
	movq %rax, -56(%rbp)
#            REPORT [tmp2]
	movq -56(%rbp), %rdi
	callq printInt
#Fn epilogue main
lbl_1: 	popq %rbp
	retq
