.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $24, %rdi
  call malloc@PLT
  movq %rax, %rbx
  movq $5, %rax
  movq %rax, 0(%rbx)
  movq $7, %rax
  movq %rax, 8(%rbx)
  movq $9, %rax
  movq %rax, 16(%rbx)
  movq %rbx, ia(%rip)

  movq $0, %rax
  movq ia(%rip), %rbx
  salq $3, %rax
  addq   %rax, %rbx
  movq   (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq $2, %rax
  movq ia(%rip), %rbx
  salq $3, %rax
  addq   %rax, %rbx
  movq   (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
