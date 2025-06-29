.data
print_fmt: .string "%ld\n"


.text

.globl f
f:
  pushq %rbp
  movq %rsp, %rbp

  movq %rdi, -8(%rbp)

  subq $8, %rsp


  movq -8(%rbp), %rax
  pushq %rax
  movq $2, %rax
  movq %rax, %rcx
  popq %rax
  imulq %rcx, %rax
 jmp .end_
  leave
  ret

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $3, %rax
 movq %rax,%rdi
  call f
  addq $8, %rsp
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  leave
  ret

.section .note.GNU-stack,"",@progbits
