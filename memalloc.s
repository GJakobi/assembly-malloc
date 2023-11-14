.section .data
brk_original: .quad 0
brk_current: .quad 0

.section .text
.global setup_brk
.type setup_brk, @function
.global dismiss_brk
.type dismiss_brk, @function
.global brk_original
.global brk_current
.global memory_alloc
.type memory_alloc, @function
.global memory_free
.type memory_free, @function

setup_brk:
    movq $0, brk_original
    movq brk_original, %rdi
    movq $12, %rax
    syscall
    movq %rax, brk_current
    movq %rax, brk_original
    ret

dismiss_brk:
    movq brk_original, %rdi
    movq $12, %rax
    syscall 
    movq %rdi, brk_current
    ret

memory_alloc:
    # argument is in rdi
    movq brk_original, %r8
    movq brk_current, %r9
    movq %rdi, %r10 # save rdi
    cmpq %r8, %r9 # if brk_current == brk_original alloc memory
    jne .dont_alloc
    # alloc new block here
    addq $16, %r9       # this is the address that needs to be returned
    movq %r9, %r12     # save the address
    addq %rdi, %r9     # now r9 contains new brk
    # here we set the brk
    movq %r9, %rdi
    movq $12, %rax
    syscall
    # now set *brk_current = 1, *(brk_current + 8) = %r10, and brk_current = %r9 
    movq brk_current, %r8
    movq $1, (%r8)
    movq %r10, 8(%r8)
    movq %r9, brk_current
  
    # now return %r9
    movq %r12, %rax
    ret

.dont_alloc:
    ret

memory_free:
    ret

