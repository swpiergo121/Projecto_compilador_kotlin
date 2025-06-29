.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $2, %rax
  pushq %rax
  movq $3, %rax
  pushq %rax
  movq $4, %rax
  popq %rcx
  imulq %rcx, %rax
  popq %rcx
  addq %rcx, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.section .note.GNU-stack,"",@progbits
