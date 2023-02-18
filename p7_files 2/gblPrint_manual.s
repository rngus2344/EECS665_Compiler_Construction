.global main
.data
gbl_a: .quad 0

.text
main:
  #prologue
  #TODO: write a prologue

  movq (gbl_a), %rdi
  callq printInt

  #epilogue
  #TODO: write a real epilogue
  retq