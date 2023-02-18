.data
.global main
.text
main: 
	pushq %rbp
	movq %rsp,%rbp
	addq $16,%rbp
	subq $16,%rsp
#Fn body main
#            [a] := 4
	movq $4, %rax
	movq %rax,-24(%rbp)
#            REPORT [a]
	movq -24(%rbp),%rdi
	callq printInt
#Fn epilogue main
lbl_0: 	addq $16,%rsp
	popq %rbp
	retq
