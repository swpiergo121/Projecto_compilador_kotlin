.data
print_fmt: .string "%ld\n"

str0: .string "hello"
s: .quad str0
str0: .string "hello"

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq s(%rip), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
