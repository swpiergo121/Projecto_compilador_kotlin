.data
print_fmt: .string "%ld\n"

la: .quad 0

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp

  movq $16, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
  movq $10000000000, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  pushq %rax
  movq $20000000000, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 8(%rax)
  popq %rax
  movq %rax, la(%rip)



  movq $0, %rax
  movq la(%rip), %rbx
  salq $3, %rax
  addq   %rax, %rbx
  movq   (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq $1, %rax
  movq la(%rip), %rbx
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
