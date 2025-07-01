.data
print_fmt: .string "%ld\n"


.text

.globl fibonacci
fibonacci:
  pushq %rbp
  movq %rsp, %rbp

  movq %rdi, -8(%rbp)

  subq $8, %rsp


  movq -8(%rbp), %rax
  pushq %rax
  movq $1, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setle %al
 movzbq %al, %rax
  cmpq $0, %rax
  je Lelse0
  subq $8, %rsp


  movq -8(%rbp), %rax
 jmp .end_fibonacci
  jmp Lend1
Lelse0:
  subq $8, %rsp


  movq -8(%rbp), %rax
  pushq %rax
  movq $1, %rax
 movq %rax, %rcx
 popq %rax
  subq %rcx, %rax
 movq %rax,%rdi
  call fibonacci
  pushq %rax
  movq -8(%rbp), %rax
  pushq %rax
  movq $2, %rax
 movq %rax, %rcx
 popq %rax
  subq %rcx, %rax
 movq %rax,%rdi
  call fibonacci
 movq %rax, %rcx
 popq %rax
  addq %rcx, %rax
 jmp .end_fibonacci
Lend1:
.end_fibonacci:
leave
ret
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $7, %rax
 movq %rax,%rdi
  call fibonacci
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
