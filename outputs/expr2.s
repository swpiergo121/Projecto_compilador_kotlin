.data
print_fmt: .string "%ld\n"

print_string: .string "%s\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $5, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setg %al
 movzbq %al, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
