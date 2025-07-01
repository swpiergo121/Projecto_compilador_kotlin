.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  subq $16, %rsp

  movq $5, %rax
  movq %rax, -8(%rbp)

  movq $0, %rax
  pushq %rax
  movq -8(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
 movzbq %al, %rax
  cmpq $0, %rax
  je Lelse0
  subq $16, %rsp


  movq -8(%rbp), %rax
  pushq %rax
  movq $1, %rax
 movq %rax, %rcx
 popq %rax
  addq %rcx, %rax
  pushq %rax
  movq -8(%rbp), %rax
  pushq %rax
  movq $1, %rax
 movq %rax, %rcx
 popq %rax
  addq %rcx, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, -16(%rbp)
  jmp Lend1
Lelse0:
  subq $16, %rsp


  movq -8(%rbp), %rax
  movq %rax, -16(%rbp)
Lend1:
  movq -16(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
