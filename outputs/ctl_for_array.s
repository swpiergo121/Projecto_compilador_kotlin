.data
print_fmt: .string "%ld\n"

print_string: .string "%s\n"

arr: .quad 0

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp

  movq $24, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
  movq $1, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  pushq %rax
  movq $2, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 8(%rax)
  pushq %rax
  movq $3, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 16(%rax)
  popq %rax
  movq %rax, arr(%rip)



 subq $8, %rsp
  movq $0, 0(%rbp)
  movq arr(%rip), %r14
Lfor0:
  movq 0(%rbp), %rax
  cmpq $3, %rax
  jge Lend1
  movq 0(%rbp), %rdx
  salq $3, %rdx
  addq %r14, %rdx
  movq (%rdx), %rax
  movq %rax, -8(%rbp)

  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq 0(%rbp), %rax
  addq $1, %rax
  movq %rax, 0(%rbp)
  jmp Lfor0
Lend1:
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
