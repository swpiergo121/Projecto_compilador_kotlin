.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



 subq $8, %rsp
  movq $0, %rax
  movq %rax, -8(%rbp)
  movq %rax, %r10
for0:
  movq -8(%rbp), %rax
  pushq %rax
  movq $2, %rax
 movq %rax, %rcx
   popq %rax
  cmpq %rcx, %rax
  sete %al
  movzbq %al, %rax
  cmpq 1, %rax
  je endfor1
  subq $8, %rsp


  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq -8(%rbp), %rax
  pushq %rax
  movq $1, %rax
 movq %rax, %rcx
   popq %rax
  addq %rcx, %rax
  movq %rax, -8(%rbp)
  jmp for0
endfor1:
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
