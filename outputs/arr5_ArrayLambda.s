.data
print_fmt: .string "%ld\n"


.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp



  movq $32, %rdi
  call malloc@PLT
  movq %rax, %rbx
  movq $0, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, 0(%rbx)
  movq $1, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, 8(%rbx)
  movq $2, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, 16(%rbx)
  movq $3, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, 24(%rbx)
  movq %rbx, arr(%rip)

  movq $0, %rax
  movq arr(%rip), %rbx
  salq $3, %rax
  addq   %rax, %rbx
  movq   (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq $3, %rax
  movq arr(%rip), %rbx
  salq $3, %rax
  addq   %rax, %rbx
  movq   (%rbx), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
