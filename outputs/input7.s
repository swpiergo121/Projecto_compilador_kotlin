.data
print_fmt: .string "%ld\n"


.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $8, %rsp
  movq $3, %rax
 movq %rax,%rdi
  movq $7, %rax
 movq %rax,%rsi
  call Point
  addq $16, %rsp
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
