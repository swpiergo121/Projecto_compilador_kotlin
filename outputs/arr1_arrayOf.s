.data
print_fmt: .string "%ld\n"

a: .quad 0

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  movq $24, %rdi
  call malloc@PLT
  movq %rax, %rbx
  movq $10, %rax
  movq %rax, 0(%rbx)
  movq $20, %rax
  movq %rax, 8(%rbx)
  movq $30, %rax
  movq %rax, 16(%rbx)
  movq %rbx, a(%rip)


  movq $1, %rax
  movq a(%rip), %rbx
  salq $3, %rax
  addq   %rax, %rbx
  movq   (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  leave
  ret

.section .note.GNU-stack,"",@progbits
