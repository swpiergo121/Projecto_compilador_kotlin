  movq $5, %rax
.data
print_fmt: .string "%ld\n"

a: .quad 0

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  movq $0, %rdi
  call malloc@PLT
  movq %rax, %rbx
  movq %rbx, a(%rip)


  movq a(%rip), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  leave
  ret

.section .note.GNU-stack,"",@progbits
