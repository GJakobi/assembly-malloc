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
    movq brk_original, %r8 # r8 contains the original brk
    movq brk_current, %r9 # r9 contains the current brk
    movq %rdi, %r10 # save rdi
    cmpq %r8, %r9 # if brk_current == brk_original alloc memory
    jne .find_free_block

alloc_new_block:
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

.find_free_block:
    movq %r8, %r11 # r11 contains the address of the first block

    .loop:
    # first, check if the first block is free
    cmpq $0, (%r11)
    jne .go_to_next_block
    # here we know that this block is free
    cmpq 8(%r11), %r10  #  - it is free, then whe check if the size in the *(current_block + 8) is >= rdi, then we should use this block
    jg .go_to_next_block
    # here we know that this block is free and it is big enough
    movq $1, (%r11) # set the first byte to 1
    # now we should calculate if remains at least 17 bytes to split the block
    movq 8(%r11), %r12 # r12 = size of current block
    subq %r10, %r12 # r12 = size of current block - rdi
    cmpq $17, %r12 # if r12 >= 17, then we can split the block
    jl .finish_without_split
    # here we know that we can split the block
    movq %r10, 8(%r11) # set the size of the first block
    addq $16, %r11 # r11 = r11 + 16
    movq %r11, %r13 # save the address of the first block
    # now we should set the second block
    addq %r10, %r11 # r11 = r11 + requested size, now r11 points to the second block
    movq $0, (%r11) # set the first byte of the second block to 0
    subq $16, %r12 # r12 = r12 - 16
    movq %r12, 8(%r11) # set the size of the second block
    # theoretically, everything is right and we can return the address of the first block
    movq %r13, %rax
    ret


# the block is not free, then we go to the next block
.go_to_next_block:
    addq 8(%r11), %r11 # r11 = r11 + size of current block
    addq $16, %r11 # r11 = r11 + 16 -> now we are at the next block
    cmpq %r9, %r11 # if r11 < r9, then we should continue the loop
    jl .loop
    # here we know that we have to alloc a new block
    jmp alloc_new_block


.finish_without_split:
    addq $16, %r11 # r11 = r11 + 16
    movq %r11, %rax
    ret


memory_free:
    # argument is in rdi
    movq $0, -16(%rdi)
    ret

