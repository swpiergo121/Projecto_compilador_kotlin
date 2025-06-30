.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $16, %rdi
  call malloc@PLT
  movq %rax, %rbx
  movq $10000000000, %rax
  movq %rax, 0(%rbx)
  movq $20000000000, %rax
  movq %rax, 8(%rbx)
  movq %rbx, la(%rip)

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
