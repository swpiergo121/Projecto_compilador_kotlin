  movq $5, %rax
.data
print_fmt: .string "%ld\n"

a: .quad 5

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq a(%rip), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
