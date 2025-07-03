.data
print_fmt: .string "%ld\n"

print_string: .string "%s\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $1, %rax
  cmpq $0, %rax
  je Lelse0

  movq $1, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  jmp Lend1
Lelse0:

  movq $0, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
Lend1:
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
