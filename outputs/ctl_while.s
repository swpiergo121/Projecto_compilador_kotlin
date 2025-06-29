.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  subq $8, %rsp

  movq $0, %rax
  movq %rax, -8(%rbp)

Lwb0:
  movq -8(%rbp), %rax
  pushq %rax
  movq $3, %rax
  popq %rcx
  cmpq %rcx, %rax
  setl %al
  movzbq %al, %rax
  cmpq $0, %rax
  je Lwe1
  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq -8(%rbp), %rax
  pushq %rax
  movq $1, %rax
  popq %rcx
  addq %rcx, %rax
  movq %rax, %r12
  movq %r12, -8(%rbp)
  jmp Lwb0
Lwe1:
.section .note.GNU-stack,"",@progbits
