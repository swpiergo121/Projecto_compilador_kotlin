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
  movq $1, %rax
  movq %rax, 0(%rbx)
  movq $2, %rax
  movq %rax, 8(%rbx)
  movq $3, %rax
  movq %rax, 16(%rbx)
  movq %rbx, arr(%rip)

  movq $0, -8(%rbp)
  movq arr(%rip), %r14
Lfor0:
  movq -8(%rbp), %rax
  cmpq $3, %rax
  jge Lend1
  movq -8(%rbp), %rdx
  salq $3, %rdx
  addq %r14, %rdx
  movq (%rdx), %rax
  movq %rax, -16(%rbp)
  movq -16(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq -8(%rbp), %rax
  addq $1, %rax
  movq %rax, -8(%rbp)
  jmp Lfor0
Lend1:
.section .note.GNU-stack,"",@progbits
