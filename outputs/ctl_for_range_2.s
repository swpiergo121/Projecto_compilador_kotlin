.data
print_fmt: .string "%ld\n"

print_string: .string "%s\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp


  subq $8, %rsp

  movq $93, %rax
  movq %rax, -8(%rbp)

  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
 subq $8, %rsp
  movq $0, %rax
  movq %rax, -16(%rbp)
for0:
  movq $2, %rax
 movq %rax, %rcx
   movq -16(%rbp), %rax
  cmpq %rcx, %rax
  movl $0, %eax
  setge %al
  movzbq %al, %rax
  cmpq $0, %rax
  jne endfor1
  subq $16, %rsp


  movq -16(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq $1, %rax
 movq %rax, %rcx
   movq -16(%rbp), %rax
  addq %rcx, %rax
  movq %rax, -16(%rbp)
  jmp for0
endfor1:
  movq $33, %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
