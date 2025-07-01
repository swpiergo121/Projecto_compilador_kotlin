.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  subq $8, %rsp

  movq $32, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
  movq $0, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  pushq %rax
  movq $1, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 8(%rax)
  pushq %rax
  movq $2, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 16(%rax)
  pushq %rax
  movq $3, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 24(%rax)
  popq %rax
  movq %rax, -8(%rbp)

  movq $0, %rax
  lea -8(%rbp), %rbx
  movq (%rbx), %rbx
  salq $3, %rax
  addq   %rax, %rbx
  movq   (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq $3, %rax
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
