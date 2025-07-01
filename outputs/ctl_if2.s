.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  subq $8, %rsp

  movq $5, %rax
  movq %rax, -8(%rbp)

  movq -8(%rbp), %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
 movzbq %al, %rax
  cmpq $0, %rax
  je Lelse0
  subq $8, %rsp


  movq $1, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  jmp Lend1
Lelse0:
  subq $8, %rsp


  movq $2, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
Lend1:
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
