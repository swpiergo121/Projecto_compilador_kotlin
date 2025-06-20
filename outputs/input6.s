  movq $42, %rax
.data
print_fmt: .string "%ld\n"

g: .quad 0

.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $8, %rsp
  movq g(%rip), %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
  leave
  ret
.section .note.GNU-stack,"",@progbits
