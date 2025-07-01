.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $8, %rax
  pushq %rax
  movq $2, %rax
 movq %rax, %rcx
 popq %rax
  cqto
  idivq %rcx
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
