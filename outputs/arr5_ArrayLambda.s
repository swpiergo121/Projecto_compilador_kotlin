.data
print_fmt: .string "%ld\n"

print_string: .string "%s\n"

arr: .quad 0

.text

.globl main
main:
  pushq %rbp
  movq %rsp, %rbp

  movq $32, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
  movq $0, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  pushq %rax
  movq $1, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 8(%rax)
  pushq %rax
  movq $2, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 16(%rax)
  pushq %rax
  movq $3, %rax
  pushq %rax
  movq $3, %rax
 movq %rax, %rcx
 popq %rax
  imulq %rcx, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 24(%rax)
  popq %rax
  movq %rax, arr(%rip)



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
