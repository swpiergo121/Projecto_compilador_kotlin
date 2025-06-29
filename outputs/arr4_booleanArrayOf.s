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
  movq %rax, %rbx
  movq $0, %rax
  movb %al, 0(%rbx)
  movq $1, %rax
  movb %al, 1(%rbx)
  movq %rbx, ba(%rip)


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
  leave
  ret

.section .note.GNU-stack,"",@progbits
