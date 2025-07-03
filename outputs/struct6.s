.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  subq $8, %rsp

  movq $16, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
  movq $7, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  popq %rax
  movq %rax, -8(%rbp)

  movq $8, %rax
  movq -8(%rbp), %rcx
  movq %rax,8 (%rcx)
  movq -8(%rbp), %rax
  movq %rax, %rbx
  movq (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq -8(%rbp), %rax
  movq %rax, %rbx
  addq $8, %rbx
  movq (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
