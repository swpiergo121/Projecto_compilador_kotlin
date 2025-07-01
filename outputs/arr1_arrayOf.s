.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  subq $8, %rsp

  movq $24, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
  movq $10, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  pushq %rax
  movq $20, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 8(%rax)
  pushq %rax
  movq $30, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 16(%rax)
  popq %rax
  movq %rax, -8(%rbp)

  movq $1, %rax
  lea -8(%rbp), %rbx
  movq (%rbx), %rbx
  salq $3, %rax
  addq   %rax, %rbx
  movq   (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
