.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  subq $8, %rsp

  movq $8, %rdi
  call malloc@PLT
  movq %rax, %rbx
  movq %rbx, %rax
  movq %rax, -8(%rbp)

  movq -8(%rbp), %rax
  movq %rax, %rbx
  movq (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.section .note.GNU-stack,"",@progbits
