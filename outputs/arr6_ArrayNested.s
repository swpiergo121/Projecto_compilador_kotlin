.data
print_fmt: .string "%ld\n"

print_string: .string "%s\n"

ia: .quad 0

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp

  movq $16, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
  movq $16, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
  movq $3, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  pushq %rax
  movq $4, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 8(%rax)
  popq %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  pushq %rax
  movq $16, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
  movq $7, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  pushq %rax
  movq $9, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 8(%rax)
  popq %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 8(%rax)
  popq %rax
  movq %rax, ia(%rip)


  subq $16, %rsp

  movq $0, %rax
  movq ia(%rip), %rbx
  salq $3, %rax
  addq   %rax, %rbx
  movq   (%rbx), %rax
  movq %rax, -8(%rbp)
  movq $1, %rax
  movq ia(%rip), %rbx
  salq $3, %rax
  addq   %rax, %rbx
  movq   (%rbx), %rax
  movq %rax, -16(%rbp)

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
  movq $0, %rax
  lea -16(%rbp), %rbx
  movq (%rbx), %rbx
  salq $3, %rax
  addq   %rax, %rbx
  movq   (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq $1, %rax
  lea -16(%rbp), %rbx
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
