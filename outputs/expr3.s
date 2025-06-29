.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $10, %rax
  pushq %rax
  movq $2, %rax
  popq %rcx
  subq %rcx, %rax
  movq %rcx, %rax
  pushq %rax
  movq $4, %rax
  popq %rcx
  movq %rcx, %rdx
  cqto
  idivq %rax
  movq %rdx, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.section .note.GNU-stack,"",@progbits
