  movq $1234567890123, %rax
.data
print_fmt: .string "%ld\n"

b: .quad 1234567890123

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq b(%rip), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
