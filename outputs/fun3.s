.data
print_fmt: .string "%ld\n"

print_string: .string "%s\n"


.text

.globl square
square:
  pushq %rbp
  movq %rsp, %rbp

  movq %rdi, -8(%rbp)

  subq $8, %rsp


  movq -8(%rbp), %rax
  pushq %rax
  movq -8(%rbp), %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
 jmp .end_square
.end_square:
leave
ret
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $2, %rax
 movq %rax,%rdi
  call square
 movq %rax,%rdi
  call square
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
