.data
.globl main
gbl_a: .quad 0
.text
main: 
	pushq %rbp
	movq %rsp, %rbp
	addq $16, %rbp
#Fn body main
#            REPORT [a]
	movq (gbl_a), %rdi
	callq printBool
#Fn epilogue main
lbl_0: 	popq %rbp
	retq
