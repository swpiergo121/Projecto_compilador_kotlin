.data
print_fmt: .string "%ld\n"

print_string: .string "%s\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



 subq $8, %rsp
  movq $0, %rax
  movq %rax, -8(%rbp)
for0:
  movq $2, %rax
 movq %rax, %rcx
   movq -8(%rbp), %rax
  cmpq %rcx, %rax
  movl $0, %eax
  setge %al
  movzbq %al, %rax
  cmpq $0, %rax
  jne endfor1

  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq $1, %rax
 movq %rax, %rcx
   movq -8(%rbp), %rax
  addq %rcx, %rax
  movq %rax, -8(%rbp)
  jmp for0
endfor1:
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
