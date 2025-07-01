.data
print_fmt: .string "%ld\n"

ba: .quad 0

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp

  movq $2, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
  movq $0, %rax
  movq %al, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  pushq %rax
  movq $1, %rax
  movq %al, %rcx
  popq %rax
  movq %rcx, 8(%rax)
  popq %rax
  movq %rax, ba(%rip)



  movq $0, %rax
  movq ba(%rip), %rbx
  lea    (%rbx,%rax,1), %rbx
  movzbq (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq $1, %rax
  movq ba(%rip), %rbx
  lea    (%rbx,%rax,1), %rbx
  movzbq (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
