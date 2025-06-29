.section .rodata
str0: .string "hello"
.text
  leaq str0(%rip), %rax
.data
print_fmt: .string "%ld\n"

s: .quad 0

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  movq $0, %rdi
  call malloc@PLT
  movq %rax, %rbx
  movq %rbx, s(%rip)


  movq s(%rip), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  leave
  ret

.section .note.GNU-stack,"",@progbits
