.data
print_fmt: .string "%ld\n"

print_string: .string "%s\n"


.text

.globl add
add:
  pushq %rbp
  movq %rsp, %rbp

  movq %rdi, -8(%rbp)
  movq %rsi, -16(%rbp)

  subq $16, %rsp


  movq -8(%rbp), %rax
  pushq %rax
  movq -16(%rbp), %rax
 movq %rax, %rcx
 popq %rax
  addq %rcx, %rax
 jmp .end_add
.end_add:
leave
ret
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $4, %rax
 movq %rax,%rdi
  movq $5, %rax
 movq %rax,%rsi
  call add
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
