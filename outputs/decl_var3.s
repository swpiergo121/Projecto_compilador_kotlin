.data
print_fmt: .string "%ld\n"

b: .quad 12
  movq $12, %rax

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  subq $8, %rsp

  movq $17, %rax
  movq %rax, -8(%rbp)

  movq b(%rip), %rax
  pushq %rax
  movq -8(%rbp), %rax
 movq %rax, %rcx
 popq %rax
  addq %rcx, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
