.data
print_fmt: .string "%ld\n"

print_string: .string "%s\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  subq $24, %rsp

  movq $8, %rdi
  call malloc@PLT
  pushq %rax
  popq %rax
  movq %rax, -8(%rbp)
  movq $8, %rdi
  call malloc@PLT
  pushq %rax
  popq %rax
  movq %rax, -16(%rbp)
  movq $8, %rdi
  call malloc@PLT
  pushq %rax
  popq %rax
  movq %rax, -24(%rbp)

  movq $8, %rax
  movq -8(%rbp), %rcx
  movq %rax,0 (%rcx)
  movq -8(%rbp), %rax
  movq -16(%rbp), %rcx
  movq %rax,0 (%rcx)
  movq -16(%rbp), %rax
  movq %rax, %rbx
  movq (%rbx), %rax
  movq %rax, -24(%rbp)
  movq -24(%rbp), %rax
  movq %rax, %rbx
  movq (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
