.data
print_fmt: .string "%ld\n"


.text
.globl sum
sum:
  pushq %rbp
  movq %rsp, %rbp
 mov %rdi, -8(%rbp)
 mov %rsi, -16(%rbp)
  subq $16, %rsp
  movq -8(%rbp), %rax
  pushq %rax
  movq -16(%rbp), %rax
  movq %rax, %rcx
  popq %rax
  addq %rcx, %rax
 jmp .end_sum
.end_sum:
  leave
  ret
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $16, %rsp
  movq $3, %rax
  movq %rax, -8(%rbp)
  movq $7, %rax
  movq %rax, -16(%rbp)
  movq -8(%rbp), %rax
 movq %rax,%rdi
  movq -16(%rbp), %rax
 movq %rax,%rsi
  call sum
  addq $16, %rsp
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
  leave
  ret
.section .note.GNU-stack,"",@progbits
