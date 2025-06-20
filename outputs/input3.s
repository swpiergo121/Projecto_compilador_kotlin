.data
print_fmt: .string "%ld\n"


.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $8, %rsp
  movq $0, %rax
  movq %rax, -8(%rbp)
  movq $0, %rax
  movq %rax, -16(%rbp)
  movq $9, %rax
  movq %rax, %r11
  movq $1, %rax
  movq %rax, %r10
Lfor0:
  movq -16(%rbp), %rcx
  cmpq %r11, %rcx
  jg Lfe1
  movq -8(%rbp), %rax
  pushq %rax
  movq -16(%rbp), %rax
  movq %rax, %rcx
  popq %rax
  addq %rcx, %rax
  movq %rax, %r12
  movq %r12, -8(%rbp)
  movq -16(%rbp), %rax
  addq %r10, %rax
  movq %rax, -16(%rbp)
  jmp Lfor0
Lfe1:
  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
  leave
  ret
.section .note.GNU-stack,"",@progbits
